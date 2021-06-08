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

#include "io.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

io::IO g_io;

namespace io {

	// ************************************************************************************
	bool IO::isDirectory(const std::string& path) {
		struct stat buf = { 0 };
		int32_t res = ::stat(path.c_str(), &buf);
		if (res == 0) {
			return S_ISDIR(buf.st_mode);
		}
		return false;
	}

	// ************************************************************************************
	bool IO::isFile(const std::string& path) {
		struct stat buf = { 0 };
		int32_t res = ::stat(path.c_str(), &buf);
		if (res == 0) {
			return S_ISREG(buf.st_mode);
		}
		return false;
	}

	// ************************************************************************************
	void IO::listFiles(const std::string& path, StringVector& files) {
		DIR* d = opendir(path.c_str());
		if (d != NULL) {
			while(true) {
				dirent* e = readdir(d);
				if (e == NULL) break;

				if (e->d_type == DT_REG) {
					files.push_back(e->d_name);
				}
			}
			closedir(d);
		}
	}

	// ************************************************************************************
	std::string IO::readFileContents(const std::string& path) {
		std::ifstream fs(path, std::ios::binary);
		if (fs.good()) {
			std::string str((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
			return str;
		} else {
			return "";
		}
	}

	// ************************************************************************************
	void IO::ensureDir(const std::string& p, bool omnitLast) {
		if (p.empty()) return;
		StringVector path = stdext::split<char>(p,"/");
		if (omnitLast) path.pop_back();
		if (path.empty()) return;

		std::stringstream ss;
		for(auto& e: path) {
			ss << e << "/";
			if (ss.str() == "/") continue;
			if (!isDirectory(ss.str())) {
				mkdir(ss.str().c_str(), 0777);
			}
		}
	}

	// ************************************************************************************
	sockaddr_in IO::resolveAddr(const std::string& host, int32_t port) {
		sockaddr_in res = { 0 };
		res.sin_family = AF_INET;
		res.sin_port = htons(port);

		if (inet_addr(host.c_str()) != INADDR_NONE) {
			res.sin_addr.s_addr = inet_addr(host.c_str());
			return res;
		}

		if (m_dnsCache.find(host) != m_dnsCache.end()) {
			res.sin_addr.s_addr = inet_addr(m_dnsCache[host].c_str());
			return res;
		}

		hostent* he = gethostbyname(host.c_str());
		if (he != NULL) {
			in_addr** addr_list = (in_addr**)he->h_addr_list;
			if (addr_list[0] != NULL) {
				res.sin_addr = *addr_list[0];
				m_dnsCache[host] = inet_ntoa(res.sin_addr);
				return res;
			}
		}

		res.sin_addr.s_addr = INADDR_NONE;
		return res;
	}

	// ************************************************************************************
	int32_t IO::createServerSocket(const std::string& host, int32_t port) {
		int32_t fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) return -1;

        int32_t en = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en));

        sockaddr_in addr = resolveAddr(host, port);
        if (addr.sin_addr.s_addr == INADDR_NONE) {
        	close(fd);
        	return -1;
        }

        if (bind(fd,(sockaddr*) &addr,sizeof(sockaddr_in)) != 0) {
        	close(fd);
        	return -1;
        }

        if (listen(fd, 10) != 0) {
        	close(fd);
        	return -1;
        }

        return fd;
	}

	// ************************************************************************************
	int32_t IO::acceptServerSocket(int32_t serverSocket, std::string& remoteAddr, int32_t& remotePort) {
		sockaddr_in addr = { 0 };
		socklen_t addrLen = sizeof(addr);
		int32_t res = accept(serverSocket, (sockaddr*)&addr, &addrLen);

		if (res >= 0) {
			remotePort = addr.sin_port;
			remoteAddr = inet_ntoa(addr.sin_addr);
		} else {
			remotePort = 0;
			remoteAddr = "";
		}

		return res;
	}

}
