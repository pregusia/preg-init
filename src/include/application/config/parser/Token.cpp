/*
 * Token.cpp
 *
 *  Created on: 26 lip 2017
 *      Author: pregusia
 */

#include "Token.h"

namespace application { namespace config { namespace parser {

	// ************************************************************************************
	std::string TokenType::toString(Enum type) {
		switch(type) {
			case TOKEN_BLOCK_CLOSE: return "BLOCK_CLOSE";
			case TOKEN_BLOCK_OPEN: return "BLOCK_OPEN";
			case TOKEN_COLON: return "COLON";
			case TOKEN_COMMA: return "COMMA";
			case TOKEN_ID: return "ID";
			case TOKEN_SEMICOLON: return "SEMICOLON";
			case TOKEN_STRING: return "STRING";
			case TOKEN_NUMBER: return "NUMBER";
			default: return stdext::format("Unknown type %d", type);
		}
	}

	// ************************************************************************************
	std::string Token::toString() const {
		if (m_data.empty()) {
			return stdext::format("[Token type=%s line=%d]", m_type, m_line);
		} else {
			return stdext::format("[Token type=%s line=%d data=\"%s\"]", m_type, m_line, m_data);
		}
	}



} } }
