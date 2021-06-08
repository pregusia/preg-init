/*
 * StreamProxy.h
 *
 *  Created on: Jun 8, 2021
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_STREAMPROXY_H_
#define INCLUDE_APPLICATION_STREAMPROXY_H_

#include "base.h"

namespace application {

	class StreamProxy: public stdext::object {
		public:
			StreamProxy(int32_t sourceFD, int32_t pipeRead, int32_t pipeWrite, int32_t destFD, const std::string& prefix);
			virtual ~StreamProxy();

			void onChildSide();
			void onParentSide();

			void onPollStart(io::FileDescriptorsPoller& poller);
			void onPollCheck(io::FileDescriptorsPoller& poller);

		private:
			std::string m_prefix;
			int32_t m_sourceFd;
			int32_t m_destFd;
			int32_t m_pipeRead;
			int32_t m_pipeWrite;
			std::vector<uint8_t> m_currentBuf;

			void processBuf(const uint8_t* buf, int32_t bufSize);
			void printCurrentAndNewBuf(const uint8_t* buf, int32_t bufStart, int32_t bufSize);

	};


}

#endif /* INCLUDE_APPLICATION_STREAMPROXY_H_ */
