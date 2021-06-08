/*
 * FileDescriptorsPoller.cpp
 *
 *  Created on: 8 lis 2017
 *      Author: pregusia
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
