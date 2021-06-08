/*
 * TokensStream.h
 *
 *  Created on: 26 lip 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_CONFIG_TOKENSSTREAM_H_
#define INCLUDE_APPLICATION_CONFIG_TOKENSSTREAM_H_

#include <base.h>
#include "Token.h"
#include "SourceStream.h"

namespace application { namespace config { namespace parser {

	class TokensStream {
		public:
			~TokensStream() { }

			int currentSourceLine() const;

			Token nextToken();
			Token peekToken();

			bool matchesArr(size_t n, TokenType::Enum types[]);

			bool matches(TokenType::Enum t1) { TokenType::Enum arr[] = { t1 }; return matchesArr(1,arr); }
			bool matches(TokenType::Enum t1, TokenType::Enum t2) { TokenType::Enum arr[] = { t1, t2 }; return matchesArr(2,arr); }
			bool matches(TokenType::Enum t1, TokenType::Enum t2, TokenType::Enum t3) { TokenType::Enum arr[] = { t1, t2, t3 }; return matchesArr(3,arr); }
			bool matches(TokenType::Enum t1, TokenType::Enum t2, TokenType::Enum t3, TokenType::Enum t4) { TokenType::Enum arr[] = { t1, t2, t3, t4 }; return matchesArr(4,arr); }
			bool matches(TokenType::Enum t1, TokenType::Enum t2, TokenType::Enum t3, TokenType::Enum t4, TokenType::Enum t5) { TokenType::Enum arr[] = { t1, t2, t3, t4, t5 }; return matchesArr(5,arr); }

			void mark() { m_markers.push_back(m_position); }
			void back() { if (!m_markers.empty()) { m_position = m_markers.back(); m_markers.pop_back(); } }
			void cancelMark() {	if (!m_markers.empty()) m_markers.pop_back(); }

			static TokensStream Tokenize(SourceStream& ss);

			const std::vector<Token>& tokens() const { return m_tokens; }

		private:
			Int32List m_markers;
			int32_t m_position;
			std::vector<Token> m_tokens;

			TokensStream();

			bool tryNumber(SourceStream& ss);
			bool tryID(SourceStream& ss);
			bool tryString(SourceStream& ss, const std::string& begin, const std::string& end);

			void add(const Token& token) { m_tokens.push_back(token); }

	};

} } }


#endif /* INCLUDE_APPLICATION_CONFIG_TOKENSSTREAM_H_ */
