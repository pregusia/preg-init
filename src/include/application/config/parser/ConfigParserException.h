/*
 * ConfigException.h
 *
 *  Created on: 26 lip 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_CONFIG_CONFIGEXCEPTION_H_
#define INCLUDE_APPLICATION_CONFIG_CONFIGEXCEPTION_H_

#include <base.h>

namespace application { namespace config { namespace parser {

	class ConfigParserException: public std::exception {
		public:
			ConfigParserException(int32_t line, const std::string& message) : m_line(line), m_message(message) { }
			ConfigParserException(const std::string& message) : m_line(0), m_message(message) { }

			virtual ~ConfigParserException() throw() { }

			virtual const char* what() const _GLIBCXX_USE_NOEXCEPT { return m_message.c_str(); }
			int32_t line() const { return m_line; }

		private:
			int32_t m_line;
			std::string m_message;
	};

} } }

#endif /* INCLUDE_APPLICATION_CONFIG_CONFIGEXCEPTION_H_ */
