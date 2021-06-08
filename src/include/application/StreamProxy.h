/*
 *
 * Copyright (c) 2021-2021 preg-init <https://github.com/pregusia/preg-init>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @author: pregusia
 *
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
