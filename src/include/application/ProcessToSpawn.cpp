/*
 * ProcessToSpawn.cpp
 *
 *  Created on: Jun 8, 2021
 *      Author: pregusia
 */

#include "ProcessToSpawn.h"
#include "StreamProxy.h"

#include <core/eventdispatcher.h>
#include <io/FileDescriptorsPoller.h>
#include <application/config/parser/ConfigEntry.h>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace application {

	// ************************************************************************************
	ProcessDeathAction::Enum ProcessDeathAction::fromString(const std::string& val) {
		if (val == "rerun") return RERUN;
		if (val == "none") return NONE;
		if (val == "stop") return STOP;
		return INVALID;
	}

	// ************************************************************************************
	int32_t signalFromString(const std::string& val) {
		auto vv = stdext::toUpperCase<char>(val);
		if (vv == "SIGHUP") return SIGHUP;
		if (vv == "SIGINT") return SIGINT;
		if (vv == "SIGQUIT") return SIGQUIT;
		if (vv == "SIGILL") return SIGILL;
		if (vv == "SIGTRAP") return SIGTRAP;
		if (vv == "SIGABRT") return SIGABRT;
		if (vv == "SIGBUS") return SIGBUS;
		if (vv == "SIGFPE") return SIGFPE;
		if (vv == "SIGKILL") return SIGKILL;
		if (vv == "SIGUSR1") return SIGUSR1;
		if (vv == "SIGSEGV") return SIGSEGV;
		if (vv == "SIGUSR2") return SIGUSR2;
		if (vv == "SIGPIPE") return SIGPIPE;
		if (vv == "SIGALRM") return SIGALRM;
		if (vv == "SIGTERM") return SIGTERM;
		if (vv == "SIGCHLD") return SIGCHLD;
		if (vv == "SIGCONT") return SIGCONT;
		if (vv == "SIGSTOP") return SIGSTOP;
		if (vv == "SIGTSTP") return SIGTSTP;
		if (vv == "SIGTTIN") return SIGTTIN;
		if (vv == "SIGTTOU") return SIGTTOU;
		if (vv == "SIGURG") return SIGURG;
		if (vv == "SIGXCPU") return SIGXCPU;
		if (vv == "SIGXFSZ") return SIGXFSZ;
		if (vv == "SIGVTALRM") return SIGVTALRM;
		if (vv == "SIGPROF") return SIGPROF;
		if (vv == "SIGWINCH") return SIGWINCH;
		if (vv == "SIGSYS") return SIGSYS;
		return -1;
	}



	// ************************************************************************************
	ProcessToSpawn::ProcessToSpawn(const std::string& name) : m_name(name) {
		m_startDelay = 0;
		m_pid = -1;
	}

	// ************************************************************************************
	ProcessToSpawn::~ProcessToSpawn() {

	}

	// ************************************************************************************
	bool ProcessToSpawn::loadFromConfig(const config::parser::ConfigEntriesCollection& config) {
		for(auto& e: config) {

			if (e->name() == "file" && e->hasValuePrimitive(0)) {
				m_execFile = e->valuePrimitive(0);
				continue;
			}
			if (e->name() == "argument" && e->hasValuePrimitive(0)) {
				m_execArgs.push_back(e->valuePrimitive(0));
				continue;
			}
			if (e->name() == "cwd" && e->hasValuePrimitive(0)) {
				m_cwd = e->valuePrimitive(0);
				continue;
			}
			if (e->name() == "start-delay" && e->hasValuePrimitive(0)) {
				m_startDelay = stdext::unsafeCast<int32_t>(e->valuePrimitive(0));
				if (m_startDelay < 0) {
					logWarning("Invalid start-delay value");
					return false;
				}
				continue;
			}
			if (e->name() == "on-death" && e->hasValuePrimitive(0)) {
				OnDeathEnty ode;
				ode.action = ProcessDeathAction::fromString(e->valuePrimitive(0));
				if (ode.action == ProcessDeathAction::INVALID) {
					logWarning("Invalid on-death action");
					return false;
				}

				if (e->valuesSize() == 3) {
					if (e->valuePrimitive(1) == "when-rc") {
						ode.rc = stdext::unsafeCast<int32_t>(e->valuePrimitive(2));
					} else {
						logWarning("Invalid on-death format");
						return false;
					}
				}

				m_onDeathEntries.push_back(ode);
				continue;
			}
			if (e->name() == "proxy" && e->hasValuePrimitive(0) && e->hasValueBlock(1)) {
				if (e->valuePrimitive(0) == "signals") {
					parseProxySignals(e->valueBlock(1));
					continue;
				}
				if (e->valuePrimitive(0) == "stdout") {
					parseProxyStream(e->valueBlock(1), STDOUT_FILENO);
					continue;
				}
				if (e->valuePrimitive(0) == "stderr") {
					parseProxyStream(e->valueBlock(1), STDERR_FILENO);
					continue;
				}
				logWarning(stdext::format("Unknown proxy mode \"%s\"", e->valuePrimitive(0)));
				continue;
			}

			logWarning(stdext::format("Unknown config entry \"%s\"", e->name()));
		}

		return true;
	}

	// ************************************************************************************
	void ProcessToSpawn::parseProxySignals(const config::parser::ConfigEntriesCollection& entries) {
		for(auto& e: entries) {
			if (e->name() == "signal" && e->hasValuePrimitive(0)) {
				SignalProxyEntry spe;
				spe.inputSinal = spe.outputSignal = signalFromString(e->valuePrimitive(0));

				if (spe.inputSinal < 0) {
					logWarning(stdext::format("Unknown signal \"%s\"", e->valuePrimitive(0)));
					continue;
				}

				if (spe.inputSinal == SIGCHLD) {
					logWarning("SIGCHLD not supported");
					continue;
				}
				if (spe.inputSinal == SIGKILL) {
					logWarning("SIGKILL not supported");
					continue;
				}

				if (e->valuesSize() == 3) {
					if (e->valuePrimitive(1) == "as") {
						spe.outputSignal = signalFromString(e->valuePrimitive(2));

						if (spe.outputSignal < 0) {
							logWarning(stdext::format("Unknown signal \"%s\"", e->valuePrimitive(2)));
							continue;
						}

						if (spe.outputSignal == SIGCHLD) {
							logWarning("SIGCHLD not supported");
							continue;
						}
					} else {
						logWarning("Unknown signal entry format");
						continue;
					}
				}

				m_signalProxies.push_back(spe);
				continue;
			}
			logWarning(stdext::format("Unknown config entry \"%s\"", e->name()));
		}
	}

	// ************************************************************************************
	void ProcessToSpawn::parseProxyStream(const config::parser::ConfigEntriesCollection& entries, int32_t fd) {
		StreamProxyEntry spe;
		spe.sourceFD = fd;

		for(auto& e: entries) {
			if (e->name() == "prefix" && e->hasValuePrimitive(0)) {
				spe.prefix = e->valuePrimitive(0);
				continue;
			}
			if (e->name() == "to" && e->hasValuePrimitive(0)) {
				spe.dest = e->valuePrimitive(0);
				continue;
			}
			logWarning(stdext::format("Unknown config entry \"%s\"", e->name()));
		}

		if (spe.dest.empty()) {
			logWarning("Empty stream proxy destination");
			return;
		}

		m_streamProxiesConf.push_back(spe);
	}

	// ************************************************************************************
	void ProcessToSpawn::logError(const std::string& text) {
		g_logger.error(stdext::format("[Proc %s] %s", m_name, text));
	}

	// ************************************************************************************
	void ProcessToSpawn::logWarning(const std::string& text) {
		g_logger.warning(stdext::format("[Proc %s] %s", m_name, text));
	}

	// ************************************************************************************
	void ProcessToSpawn::logInfo(const std::string& text) {
		g_logger.info(stdext::format("[Proc %s] %s", m_name, text));
	}


	// ************************************************************************************
	void ProcessToSpawn::start() {
		if (isRunning()) return;

		if (m_startDelay > 0) {
			auto self = dynamic_self_cast<ProcessToSpawn>();
			g_dispatcher.scheduleEvent(std::bind(&ProcessToSpawn::startNow, self), m_startDelay * 1000);
		} else {
			startNow();
		}
	}

	// ************************************************************************************
	void ProcessToSpawn::startNow() {
		if (isRunning()) return;

		// stream proxies
		for(auto& e: m_streamProxiesConf) {
			int32_t destFD = e.prepareDestFD();
			if (destFD < 0) {
				logError("Cannot prepare destination FD");
				return;
			}

			int32_t pipeFDs[2] = { 0 };
			int32_t res = ::pipe(pipeFDs);

			if (res < 0) {
				perror("pipe");
				if (destFD != STDOUT_FILENO && destFD != STDERR_FILENO) {
					::close(destFD);
				}
				logError("pipe failed");
				return;
			}

			StreamProxyPtr proxy(new StreamProxy(e.sourceFD, pipeFDs[0], pipeFDs[1], destFD, e.prefix));
			m_streamProxies.push_back(proxy);
		}


		// fork
		int32_t res = ::fork();
		if (res < 0) {
			perror("fork");
			m_streamProxies.clear();
			logError("fork failed");
			return;
		}

		if (res == 0) {
			// child

			for(auto& e: m_streamProxies) {
				e->onChildSide();
			}

			if (!m_cwd.empty()) {
				chdir(m_cwd.c_str());
				// TODO: check return val
			}

			// TODO: powinnismy chyba przywrocic signal block mask na oryginalna
			// TODO: jakeis process group, tty itp

			char* argv[64] = { 0 };
			int32_t argvLen = 0;
			argv[argvLen++] = strdup(basename(m_execFile.c_str()));
			for(size_t i=0;i<m_execArgs.size();++i) {
				argv[argvLen++] = strdup(m_execArgs[i].c_str());
			}

			execv(m_execFile.c_str(), argv);
			perror("execve");
			exit(1);
		} else {
			// parent
			m_pid = res;
			for(auto& e: m_streamProxies) {
				e->onParentSide();
			}

			logInfo(stdext::format("Started as pid %d", res));
		}
	}

	// ************************************************************************************
	void ProcessToSpawn::doKill() {
		if (!isRunning()) return;

		// TODO: powinna byc chyba lepsza logika, ze wysylamy, czekamy itp

		::kill(m_pid, SIGKILL);
	}

	// ************************************************************************************
	void ProcessToSpawn::onSignal(int32_t sig) {
		if (!isRunning()) return;

		// ok, myslimy co z tym zrobic
		for(auto& e: m_signalProxies) {
			if (e.inputSinal == sig) {
				logInfo(stdext::format("Sending signal %d to pid %d (in response to signal %d)",
					e.outputSignal,
					m_pid,
					sig
				));
				kill(m_pid, e.outputSignal);
				return;
			}
		}

		// znaczy ze nic nie robimy i ignorujemy
	}

	// ************************************************************************************
	ProcessDeathAction::Enum ProcessToSpawn::onKilled(int32_t pid, int32_t exitCode) {
		if (!isRunning()) return ProcessDeathAction::NONE;
		if (pid != m_pid) return ProcessDeathAction::NONE;

		logInfo(stdext::format("Stopped with exitCode %d", exitCode));

		ProcessDeathAction::Enum action = ProcessDeathAction::INVALID;

		for(auto& e: m_onDeathEntries) {
			if (e.rc == -1) continue;
			if (e.rc == exitCode) {
				action = e.action;
				break;
			}
		}

		if (action == ProcessDeathAction::INVALID) {
			for(auto& e: m_onDeathEntries) {
				if (e.rc == -1) {
					action = e.action;
					break;
				}
			}
		}

		if (action == ProcessDeathAction::INVALID) {
			action = ProcessDeathAction::NONE;
		}

		m_pid = -1;
		m_streamProxies.clear();

		return action;
	}

	// ************************************************************************************
	void ProcessToSpawn::onProxiesPollStart(io::FileDescriptorsPoller& poller) {
		for(auto& e: m_streamProxies) {
			e->onPollStart(poller);
		}
	}

	// ************************************************************************************
	void ProcessToSpawn::onProxiesPollCheck(io::FileDescriptorsPoller& poller) {
		for(auto& e: m_streamProxies) {
			e->onPollCheck(poller);
		}
	}


	// ************************************************************************************
	int32_t ProcessToSpawn::StreamProxyEntry::prepareDestFD() {
		if (dest == "-") return STDOUT_FILENO;
		if (dest == "&1") return STDOUT_FILENO;
		if (stdext::toLowerCase(dest) == "stdout") return STDOUT_FILENO;
		if (dest == "&2") return STDERR_FILENO;
		if (stdext::toLowerCase(dest) == "stderr") return STDERR_FILENO;

		// znaczy ze plik
		return ::open(dest.c_str(), O_CREAT, 0600);
	}

}
