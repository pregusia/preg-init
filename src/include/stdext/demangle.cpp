
#include "demangle.h"
#include <cxxabi.h>
#include <cstring>
#include <cstdlib>
#include <sstream>

namespace stdext {

	const std::string& demangled_name::last() const {
		if (m_parts.empty()) {
			static std::string empty;
			return empty;
		} else {
			return m_parts.back();
		}
	}

	std::string demangled_name::full() const {
		std::stringstream ss;
		bool first = true;

		for(auto it=m_parts.begin();it != m_parts.end();++it) {
			if (!first) {
				ss << "::";
			}
			ss << *it;
			first = false;
		}

		return ss.str();
	}

	demangled_name demangled_name::createFromString(const char* name) {
		size_t len;
		int status;
		char* buf = abi::__cxa_demangle(name, NULL, &len, &status);
		demangled_name res;

		if (buf != NULL) {
			std::string str(buf);
			size_t pos = 0;
			std::string token;
			std::string delimiter = "::";

			while((pos = str.find(delimiter)) != std::string::npos) {
			    std::string token = str.substr(0, pos);
			    if (!token.empty()) {
			    	res.m_parts.push_back(token);
			    }
			    str.erase(0, pos + delimiter.length());
			}

			if (!str.empty()) {
				res.m_parts.push_back(str);
			}

			free(buf);
		}

		return res;
	}

}
