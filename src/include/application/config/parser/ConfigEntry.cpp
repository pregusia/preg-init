/*
 * ConfigEntry.cpp
 *
 *  Created on: 26 lip 2017
 *      Author: pregusia
 */

#include "ConfigEntry.h"
#include "ConfigParserException.h"

namespace application { namespace config { namespace parser {

	// ************************************************************************************
	const std::string& ConfigEntry::valuePrimitive(int32_t nr) {
		if (hasValuePrimitive(nr)) {
			return value(nr)->primitive();
		} else {
			static std::string empty;
			return empty;
		}
	}

	// ************************************************************************************
	const ConfigEntriesCollection& ConfigEntry::valueBlock(int32_t nr) {
		if (hasValueBlock(nr)) {
			return value(nr)->block();
		} else {
			static ConfigEntriesCollection empty;
			return empty;
		}
	}


	// ************************************************************************************
	ConfigEntryPtr ConfigEntriesCollection::entry(const std::string& name) {
		for(auto& entry: m_entries) {
			if (entry->name() == name) return entry;
		}
		return nullptr;
	}

	// ************************************************************************************
	ConfigEntriesCollection::container ConfigEntriesCollection::entries(const std::string& name) {
		container res;

		for(auto& entry: m_entries) {
			if (entry->name() == name) {
				res.push_back(entry);
			}
		}

		return res;
	}

	// ************************************************************************************
	ConfigEntryPtr ConfigEntriesCollection::parseEntry(TokensStream& tokens) {
		if (!tokens.matches(TokenType::TOKEN_ID)) return nullptr;
		tokens.mark();

		ConfigEntryPtr entry(new ConfigEntry(tokens.nextToken().asString()));

		while(true) {

			if (tokens.matches(TokenType::TOKEN_SEMICOLON)) {
				// znaczy ze koniec tego wpisu
				tokens.nextToken();
				tokens.cancelMark();
				break;
			}

			if (tokens.matches(TokenType::TOKEN_NUMBER)) {
				auto val = tokens.nextToken().asString();
				entry->values().push_back(ConfigEntryValuePtr(new ConfigEntryValue(val)));
				continue;
			}

			if (tokens.matches(TokenType::TOKEN_STRING)) {
				auto val = tokens.nextToken().asString();
				entry->values().push_back(ConfigEntryValuePtr(new ConfigEntryValue(val)));
				continue;
			}

			if (tokens.matches(TokenType::TOKEN_ID)) {
				auto val = tokens.nextToken().asString();
				entry->values().push_back(ConfigEntryValuePtr(new ConfigEntryValue(val)));
				continue;
			}

			if (tokens.matches(TokenType::TOKEN_BLOCK_OPEN)) {
				tokens.nextToken();

				auto val = ConfigEntriesCollection::Parse(tokens);

				if (tokens.matches(TokenType::TOKEN_BLOCK_CLOSE)) {
					tokens.nextToken();
					entry->values().push_back(ConfigEntryValuePtr(new ConfigEntryValue(val)));
					continue;
				} else {
					int32_t line = tokens.currentSourceLine();
					tokens.back();
					throw ConfigParserException(line,"Expecting }");
				}
			}

			throw ConfigParserException(tokens.peekToken().line(),"Invalid token");
		}

		return entry;
	}

	// ************************************************************************************
	ConfigEntriesCollection ConfigEntriesCollection::Parse(TokensStream& tokens) {
		ConfigEntriesCollection res;

		while(true) {
			auto entry = parseEntry(tokens);
			if (!entry.empty()) {
				res.m_entries.push_back(entry);
				continue;
			} else {
				break;
			}
		}

		return res;
	}

} } }
