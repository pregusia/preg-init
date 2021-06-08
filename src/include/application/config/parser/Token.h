/*
 * Token.h
 *
 *  Created on: 26 lip 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_CONFIG_TOKEN_H_
#define INCLUDE_APPLICATION_CONFIG_TOKEN_H_

#include <base.h>

namespace application { namespace config { namespace parser {

	class TokenType {
		public:

			typedef enum {
				TOKEN_ID = 1,

				TOKEN_STRING = 10,
				TOKEN_NUMBER = 11,

				TOKEN_BLOCK_OPEN = 20, // {
				TOKEN_BLOCK_CLOSE = 21, // }

				TOKEN_SEMICOLON = 70, // ;
				TOKEN_COLON = 71, // :
				TOKEN_COMMA = 72, // ,


				TOKEN_END = 255
			} Enum;

			static std::string toString(Enum type);

		private:
			TokenType() { }

	};


	class Token {
		public:
			Token(int line, TokenType::Enum type) : m_line(line), m_type(type), m_data("") { };
			Token(int line, TokenType::Enum type, const std::string& data) : m_line(line), m_type(type), m_data(data) { };

			~Token() { }
			std::string toString() const;


			int32_t line() const { return m_line; }
			TokenType::Enum type() const { return m_type; }

			const std::string& asString() const { return m_data; }
			const int32_t asInt() const { return atoi(m_data.c_str()); }
			const float asFloat() const { return atof(m_data.c_str()); }

		private:
			int32_t m_line;
			TokenType::Enum m_type;
			std::string m_data;
	};

} } }


#endif /* INCLUDE_APPLICATION_CONFIG_TOKEN_H_ */
