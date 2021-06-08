/*
 * FileDescriptorsPoller.h
 *
 *  Created on: 8 lis 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_IO_FILEDESCRIPTORSPOLLER_H_
#define INCLUDE_IO_FILEDESCRIPTORSPOLLER_H_

#include <base.h>
#include <sys/select.h>

namespace io {

	class FileDescriptorsPoller {
		public:
			FileDescriptorsPoller();

			void watchRead(int32_t fd);
			void watchWrite(int32_t fd);

			bool poll(int32_t ms=-1);

			bool canRead(int32_t fd) const;
			bool canWrite(int32_t fd) const;

		private:
			FileDescriptorsPoller(const FileDescriptorsPoller& from);

			fd_set m_readSet;
			fd_set m_writeSet;
			int32_t m_maxFd;
	};


}

#endif /* INCLUDE_IO_FILEDESCRIPTORSPOLLER_H_ */
