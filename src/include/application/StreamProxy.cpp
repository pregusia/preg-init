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

#include "StreamProxy.h"

#include <io/FileDescriptorsPoller.h>

namespace application {

	// ************************************************************************************
	StreamProxy::StreamProxy(int32_t sourceFD, int32_t pipeRead, int32_t pipeWrite, int32_t destFD, const std::string& prefix) {
		m_sourceFd = sourceFD; // to jest tylko informacyjne
		m_destFd = destFD;
		m_pipeRead = pipeRead;
		m_pipeWrite = pipeWrite;
		m_prefix = prefix;
	}

	// ************************************************************************************
	StreamProxy::~StreamProxy() {
		if (m_pipeWrite > -1) {
			::close(m_pipeWrite);
			m_pipeWrite = -1;
		}
		if (m_pipeRead > -1) {
			::close(m_pipeRead);
			m_pipeRead = -1;
		}
		if (m_destFd > -1 && m_destFd != STDOUT_FILENO && m_destFd != STDERR_FILENO) {
			::close(m_destFd);
			m_destFd = -1;
		}
	}

	// ************************************************************************************
	void StreamProxy::onChildSide() {
		::dup2(m_pipeWrite, m_sourceFd);
		::close(m_pipeWrite); m_pipeWrite = -1;
		::close(m_pipeRead); m_pipeRead = -1;
		if (m_destFd != STDOUT_FILENO && m_destFd != STDERR_FILENO) {
			::close(m_destFd); m_destFd = -1;
		}

		// zostaje nam m_pipeWrite zmapowane jako m_sourceFd
	}

	// ************************************************************************************
	void StreamProxy::onParentSide() {
		::close(m_pipeWrite); m_pipeWrite = -1;

		// zostaje nam m_pipeRead i m_destFd
	}

	// ************************************************************************************
	void StreamProxy::onPollStart(io::FileDescriptorsPoller& poller) {
		if (m_pipeRead > -1) {
			poller.watchRead(m_pipeRead);
		}
	}

	// ************************************************************************************
	void StreamProxy::onPollCheck(io::FileDescriptorsPoller& poller) {
		if (m_pipeRead > -1 && poller.canRead(m_pipeRead)) {

			uint8_t buf[1024] = { 0 };
			int32_t res = ::read(m_pipeRead, buf, 1024);
			if (res <= 0) {
				// koniec/blad
				m_pipeRead = -1;
				return;
			}

			processBuf(buf, res);
		}
	}

	// ************************************************************************************
	void StreamProxy::processBuf(const uint8_t* buf, int32_t bufSize) {
		int32_t bufStart = 0;
		for(int32_t i=0;i<bufSize;++i) {
			if (buf[i] == '\n') {
				bufStart = i + 1;
				printCurrentAndNewBuf(buf, 0, i + 1);
				processBuf(buf + i + 1, bufSize - i - 1);
				return;
			}
		}

		// nie ma \n, wiec dopisujemy
		// XXX: pewno da sie lepiej to zrobic
		for(int32_t i=bufStart;i<bufSize;++i) {
			m_currentBuf.push_back(buf[i]);
		}
	}

	// ************************************************************************************
	void StreamProxy::printCurrentAndNewBuf(const uint8_t* buf, int32_t bufStart, int32_t bufSize) {
		// to zaklada, ze ::write sie powiedzie i nie bedzie blokujace

		if (!m_prefix.empty()) {
			::write(m_destFd, m_prefix.c_str(), m_prefix.size());
		}

		for(size_t i=0;i<m_currentBuf.size();) {
			int32_t res = ::write(m_destFd, &m_currentBuf[i], m_currentBuf.size() - i);
			if (res <= 0) break;
			i += res;
		}

		m_currentBuf.clear();

		for(int32_t i=bufStart;i<bufSize;) {
			int32_t res = ::write(m_destFd, &buf[i], bufSize - i);
			if (res <= 0) break;
			i += res;
		}
	}


}
