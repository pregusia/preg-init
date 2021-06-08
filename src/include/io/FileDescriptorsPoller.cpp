/*
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


#include "FileDescriptorsPoller.h"

namespace io {

	// ************************************************************************************
	FileDescriptorsPoller::FileDescriptorsPoller() {
		FD_ZERO(&m_readSet);
		FD_ZERO(&m_writeSet);
		m_maxFd = -1;
	}

	// ************************************************************************************
	void FileDescriptorsPoller::watchRead(int32_t fd) {
		FD_SET(fd, &m_readSet);
		m_maxFd = std::max(m_maxFd, fd);
	}

	// ************************************************************************************
	void FileDescriptorsPoller::watchWrite(int32_t fd) {
		FD_SET(fd, &m_writeSet);
		m_maxFd = std::max(m_maxFd, fd);
	}

	// ************************************************************************************
	bool FileDescriptorsPoller::poll(int32_t ms) {
		int32_t res = -1;

		if (ms > 0) {
			struct timeval tv;
			tv.tv_sec = ms / 1000;
			tv.tv_usec = (ms % 1000) * 1000;

			res = ::select(m_maxFd + 1, &m_readSet, &m_writeSet, nullptr, &tv);
		} else {
			res = ::select(m_maxFd + 1, &m_readSet, &m_writeSet, nullptr, nullptr);
		}

		if (res <= 0) {
			FD_ZERO(&m_readSet);
			FD_ZERO(&m_writeSet);
		}

		return res > 0;
	}

	// ************************************************************************************
	bool FileDescriptorsPoller::canRead(int32_t fd) const {
		return FD_ISSET(fd, &m_readSet);
	}

	// ************************************************************************************
	bool FileDescriptorsPoller::canWrite(int32_t fd) const {
		return FD_ISSET(fd, &m_writeSet);
	}


}
