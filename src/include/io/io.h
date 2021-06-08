/*
 * io.h
 *
 *  Created on: 27 sie 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_IO_IO_H_
#define INCLUDE_IO_IO_H_

#include <base.h>
#include <arpa/inet.h>

namespace io {

	class IO {

		public:
			void listFiles(const std::string& path, StringVector& files);
			bool isFile(const std::string& path);
			bool isDirectory(const std::string& path);
			std::string readFileContents(const std::string& path);
			void ensureDir(const std::string& path, bool omnitLast);

			sockaddr_in resolveAddr(const std::string& name, int32_t port);

			int32_t createServerSocket(const std::string& host, int32_t port);
			int32_t acceptServerSocket(int32_t serverSocket, std::string& remoteAddr, int32_t& remotePort);

		private:

			std::map<std::string,std::string> m_dnsCache;
	};

}

extern io::IO g_io;

#endif /* INCLUDE_IO_IO_H_ */
