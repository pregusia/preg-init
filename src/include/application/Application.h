/*
 * Application.h
 *
 *  Created on: 27 sie 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_APPLICATION_H_
#define INCLUDE_APPLICATION_APPLICATION_H_

#include "base.h"

#include <signal.h>

namespace application {

	class Application {
		public:
			Application();
			~Application();

			int32_t run(const std::string& configFile);

		private:
			volatile bool m_running;
			std::vector<ProcessToSpawnPtr> m_processes;
			sigset_t m_signalsMask;
			int32_t m_endSignal;

			bool processConfig(const std::string& path);

			void registerSignals();
			void pollUnixSignals();
			void pollStreamProxies();


	};

}

extern application::Application g_app;

#endif /* INCLUDE_APPLICATION_APPLICATION_H_ */
