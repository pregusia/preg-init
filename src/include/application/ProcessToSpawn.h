/*
 * ProcessToSpawn.h
 *
 *  Created on: Jun 8, 2021
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_PROCESSTOSPAWN_H_
#define INCLUDE_APPLICATION_PROCESSTOSPAWN_H_

#include "base.h"

namespace application {

	class ProcessToSpawn: public stdext::object {
		public:
			ProcessToSpawn(const std::string& name);
			virtual ~ProcessToSpawn();

			const std::string& getName() const { return m_name; }
			bool isRunning() const { return m_pid > 0; }

			bool loadFromConfig(const config::parser::ConfigEntriesCollection& config);

			void start();
			void startNow();
			void doKill();

			void onSignal(int32_t sig);
			ProcessDeathAction::Enum onKilled(int32_t pid, int32_t exitCode);
			void onProxiesPollStart(io::FileDescriptorsPoller& poller);
			void onProxiesPollCheck(io::FileDescriptorsPoller& poller);

			void logError(const std::string& text);
			void logWarning(const std::string& text);
			void logInfo(const std::string& text);

		private:

			void parseProxySignals(const config::parser::ConfigEntriesCollection& entries);
			void parseProxyStream(const config::parser::ConfigEntriesCollection& entries, int32_t fd);


			class OnDeathEnty {
				public:
					ProcessDeathAction::Enum action;
					int32_t rc;

					OnDeathEnty() : action(ProcessDeathAction::INVALID), rc(-1) { }
			};

			class SignalProxyEntry {
				public:
					int32_t inputSinal;
					int32_t outputSignal;

					SignalProxyEntry() : inputSinal(0), outputSignal(0) { }
			};

			class StreamProxyEntry {
				public:
					int32_t sourceFD;
					std::string prefix;
					std::string dest;

					StreamProxyEntry() : sourceFD(0) { }
					int32_t prepareDestFD();
			};

			std::string m_name;
			std::string m_execFile;
			StringVector m_execArgs;
			std::string m_cwd;
			int32_t m_startDelay;
			std::vector<OnDeathEnty> m_onDeathEntries;
			std::vector<SignalProxyEntry> m_signalProxies;
			std::vector<StreamProxyEntry> m_streamProxiesConf;
			std::vector<StreamProxyPtr> m_streamProxies;

			pid_t m_pid;
	};

}

#endif /* INCLUDE_APPLICATION_PROCESSTOSPAWN_H_ */
