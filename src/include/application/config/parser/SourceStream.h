/*
 * SourceStream.h
 *
 *  Created on: 26 lip 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_CONFIG_SOURCESTREAM_H_
#define INCLUDE_APPLICATION_CONFIG_SOURCESTREAM_H_

#include <base.h>

namespace application { namespace config { namespace parser {

	class SourceStream {
		public:
			SourceStream(const std::string& data) : m_data(data), m_position(0) { }
			~SourceStream() { }

			bool hasNext() const { return m_position < m_data.length() - 1; }

			char next();
			std::string next(int32_t n);

			char peek() const;
			std::string peek(int32_t n) const;

			void skipToLineEnd();
			void skipWhitespaces();

			int32_t currentLine() const;
			int32_t position() const { return m_position; }

			bool matches(const std::string& val) const;

			void mark() { m_markers.push_back(m_position); }
			void back() { if (!m_markers.empty()) { m_position = m_markers.back(); m_markers.pop_back(); } }

			static bool isWhitespace(char c) { return c == '\t' || c == '\n' || c == ' ' || c == '\r'; }
			static bool isAlph(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
			static bool isNum(char c) { return c >= '0' && c <= '9'; }

		private:
			std::string m_data;
			size_t m_position;
			Int32Vector m_markers;
	};

} } }

#endif /* INCLUDE_APPLICATION_CONFIG_SOURCESTREAM_H_ */
