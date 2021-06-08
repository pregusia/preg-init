/*
 * SourceStream.cpp
 *
 *  Created on: 26 lip 2017
 *      Author: pregusia
 */

#include "SourceStream.h"

namespace application { namespace config { namespace parser {

	// ************************************************************************************
	char SourceStream::next() {
		if (m_position < m_data.length()) {
			return m_data[m_position++];
		} else {
			return 0;
		}
	}

	// ************************************************************************************
	std::string SourceStream::next(int32_t n) {
		std::stringstream ss;
		for(int32_t i=0;i<n;++i) {
			char ch = next();
			if (ch == 0) break;
			ss.put(ch);
		}
		return ss.str();
	}

	// ************************************************************************************
	char SourceStream::peek() const {
		if (m_position < m_data.length()) {
			return m_data[m_position];
		} else {
			return 0;
		}
	}

	// ************************************************************************************
	std::string SourceStream::peek(int32_t n) const {
		std::stringstream ss;
		for(int32_t i=0;i<n;++i) {
			if (m_position + i < m_data.length()) {
				ss.put(m_data[m_position + i]);
			} else {
				break;
			}
		}
		return ss.str();
	}

	// ************************************************************************************
	void SourceStream::skipToLineEnd() {
		while(hasNext()) {
			char c = next();
			if (c == '\n') return;
		}
	}

	// ************************************************************************************
	void SourceStream::skipWhitespaces() {
		while(true) {
			char c = peek();
			if (c == 0) return;
			if (isWhitespace(c)) {
				next();
			} else {
				return;
			}
		}
	}

	// ************************************************************************************
	int SourceStream::currentLine() const {
		int32_t line = 1;
		for(size_t i=0;i<m_position;++i) {
			if (m_data[i] == '\n') ++line;
		}
		return line;
	}


	// ************************************************************************************
	bool SourceStream::matches(const std::string& val) const {
		if (val.empty()) return true;
		return peek(val.length()) == val;
	}

} } }
