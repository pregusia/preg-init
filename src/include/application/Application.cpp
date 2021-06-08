/*
 * Application.cpp
 *
 *  Created on: 27 sie 2017
 *      Author: pregusia
 */

#include "Application.h"

#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <core/eventdispatcher.h>
#include <io/io.h>
#include <io/FileDescriptorsPoller.h>

#include <application/config/parser/ConfigParserException.h>
#include <application/config/parser/ConfigEntry.h>
#include <application/config/parser/Token.h>
#include <application/config/parser/TokensStream.h>


#include "ProcessToSpawn.h"

application::Application g_app;

namespace application {

	int32_t signalFromString(const std::string& val);

	// ************************************************************************************
	Application::Application() {
		m_running = false;
		m_endSignal = -1;
	}

	// ************************************************************************************
	Application::~Application() {

	}

	// ************************************************************************************
	int32_t Application::run(const std::string& configFile) {
		registerSignals();

	    try {
			// scripting
			if (!processConfig(configFile)) {
				g_logger.error("Cannot process config");
				return 1;
			}

			if (m_processes.empty()) {
				g_logger.warning("No processes defined. Exiting");
				return 0;
			}

			m_running = true;
			g_logger.info("Application started");

			for(auto& proc: m_processes) {
				proc->start();
			}

			while(m_running) {
				g_clock.update();

				pollStreamProxies();
				pollUnixSignals();

				g_dispatcher.poll(true);
			}

			for(auto& proc: m_processes) {
				proc->doKill();
			}

			g_dispatcher.shutdown();
			g_logger.info("Application stopped");

	    } catch(const core::Exception& e) {
			g_logger.warning(stdext::format("EXCEPTION:\n%s", e.what()));
			return 1;
		} catch(const std::exception& e) {
			g_logger.warning(stdext::format("EXCEPTION:\n%s", e.what()));
			return 1;
		}

		return 0;
	}

	// ************************************************************************************
	bool Application::processConfig(const std::string& path) {
		if (!g_io.isFile(path)) {
			g_logger.warning(stdext::format("Could not open config file %s", path));
			return false;
		}

		auto content = g_io.readFileContents(path);

		try {
			config::parser::SourceStream source(content);
			config::parser::TokensStream tokens = config::parser::TokensStream::Tokenize(source);
			config::parser::ConfigEntriesCollection config = config::parser::ConfigEntriesCollection::Parse(tokens);

			for(auto& e: config) {

				if (e->name() == "end-signal" && e->hasValuePrimitive(0)) {
					m_endSignal = signalFromString(e->valuePrimitive(0));
				}

				if (e->name() == "spawn" && e->hasValuePrimitive(0) && e->hasValueBlock(1)) {
					auto name = e->valuePrimitive(0);

					if (!name.empty()) {

						ProcessToSpawnPtr proc(new ProcessToSpawn(name));
						if (proc->loadFromConfig(e->valueBlock(1))) {
							m_processes.push_back(proc);
						} else {
							g_logger.error(stdext::format("Cannot parse config for spawn %s", name));
							return false;
						}

					} else {
						g_logger.error("Empty spawn name");
						return false;
					}
				}
			}

		}
		catch(const config::parser::ConfigParserException& e) {
			g_logger.error(stdext::format("ConfigParserException: %s at line %d\n", e.what(), e.line()));
			return false;
		}
		catch(const std::exception& e) {
			g_logger.error(stdext::format("Exception: %s\n", e.what()));
			return false;
		}

		return true;
	}

	// ************************************************************************************
	void Application::registerSignals() {

		sigemptyset(&m_signalsMask);
		sigaddset(&m_signalsMask, SIGHUP);
		sigaddset(&m_signalsMask, SIGINT);
		sigaddset(&m_signalsMask, SIGQUIT);
		sigaddset(&m_signalsMask, SIGILL);
		sigaddset(&m_signalsMask, SIGTRAP);
		sigaddset(&m_signalsMask, SIGABRT);
		sigaddset(&m_signalsMask, SIGBUS);
		sigaddset(&m_signalsMask, SIGFPE);
		//sigaddset(&m_signalsMask, SIGKILL);
		sigaddset(&m_signalsMask, SIGUSR1);
		sigaddset(&m_signalsMask, SIGSEGV);
		sigaddset(&m_signalsMask, SIGUSR2);
		sigaddset(&m_signalsMask, SIGPIPE);
		sigaddset(&m_signalsMask, SIGALRM);
		sigaddset(&m_signalsMask, SIGTERM);
		sigaddset(&m_signalsMask, SIGCHLD);
		sigaddset(&m_signalsMask, SIGCONT);
		sigaddset(&m_signalsMask, SIGSTOP);
		sigaddset(&m_signalsMask, SIGTSTP);
		sigaddset(&m_signalsMask, SIGTTIN);
		sigaddset(&m_signalsMask, SIGTTOU);
		sigaddset(&m_signalsMask, SIGURG);
		sigaddset(&m_signalsMask, SIGXCPU);
		sigaddset(&m_signalsMask, SIGXFSZ);
		sigaddset(&m_signalsMask, SIGVTALRM);
		sigaddset(&m_signalsMask, SIGPROF);
		sigaddset(&m_signalsMask, SIGWINCH);
		sigaddset(&m_signalsMask, SIGSYS);

		if (sigprocmask(SIG_BLOCK, &m_signalsMask, nullptr) < 0) {
			perror("sigprocmask");
			throw core::Exception("sigprocmask failed");
		}
	}

	// ************************************************************************************
	void Application::pollUnixSignals() {
		struct timespec timeout;

		timeout.tv_sec = 0;
		timeout.tv_nsec = 0;

		int32_t sig = sigtimedwait(&m_signalsMask, nullptr, &timeout);
		if (sig > 0) {
			// mamy sygnal!

			if (sig != SIGCHLD) {
				g_logger.info(stdext::format("Got signal %d", sig));
			}

			if (sig == m_endSignal) {
				g_logger.info("Stopping application");
				m_running = false;
				return;
			}

			if (sig == SIGCHLD) {
				int32_t status = 0;
				int32_t pid = ::wait(&status);

				for(auto& proc: m_processes) {
					auto res = proc->onKilled(pid, WEXITSTATUS(status));

					if (res == ProcessDeathAction::RERUN) {
						g_logger.info(stdext::format("Scheduling restart of \"%s\"", proc->getName()));
						proc->start();
					}
					if (res == ProcessDeathAction::STOP) {
						g_logger.info(stdext::format("Stopping application after death of \"%s\"", proc->getName()));
						m_running = false;
					}
				}

			} else {
				for(auto& proc: m_processes) {
					proc->onSignal(sig);
				}
			}
		}
	}

	// ************************************************************************************
	void Application::pollStreamProxies() {
		io::FileDescriptorsPoller poller;

		for(auto& proc: m_processes) {
			proc->onProxiesPollStart(poller);
		}

		poller.poll(100);

		for(auto& proc: m_processes) {
			proc->onProxiesPollCheck(poller);
		}
	}


}
