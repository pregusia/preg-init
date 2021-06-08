/*
 * string.h
 *
 *  Created on: 9 wrz 2015
 *      Author: pregusia
 */

#ifndef INCLUDE_STDEXT_STRING_H_
#define INCLUDE_STDEXT_STRING_H_

#include <string>
#include <boost/algorithm/string.hpp>
#include "casts.h"
#include <vector>
#include <set>
#include <list>

namespace stdext {

	// ************************************************************************************
	template<typename T>
	static std::string toString(const T& t) { return unsafeCast<std::string, T>(t); }

	// ************************************************************************************
	int32_t toInt(const std::string& v);
	float toFloat(const std::string& v);
	bool toBoolean(std::string v);

	// ************************************************************************************
	bool isNumeric(const std::string& v);

	// ************************************************************************************
	template<typename T>
	static std::u16string toU16String(const T& t) {
		auto s = unsafeCast<std::string, T>(t);
		return unsafeCast<std::u16string>(s);
	}

	// ************************************************************************************
	template<typename T>
	T fromString(const std::string& str, T def = T()) { return unsafeCast<T, std::string>(str, def); }

	// ************************************************************************************
	std::string pathDirectory(const std::string& filePath);
	std::string pathResolve(const std::string& filePath, std::string sourcePath);

	// ************************************************************************************
	uint64_t hexToDec(const std::string& str);

	// ************************************************************************************
	char upperCharacter(char c);
	char16_t upperCharacter(char16_t c);

	// ************************************************************************************
	char lowerCharacter(char c);
	char16_t lowerCharacter(char16_t c);

	// ************************************************************************************
	template<typename C>
	std::basic_string<C> toLowerCase(const std::basic_string<C>& str) {
		std::basic_string<C> res = str;
		for(typename std::basic_string<C>::size_type i=0;i<res.length();++i) {
			res[i] = lowerCharacter(res[i]);
		}
		return res;
	}

	// ************************************************************************************
	template<typename C>
	std::basic_string<C> toUpperCase(const std::basic_string<C>& str) {
		std::basic_string<C> res = str;
		for(typename std::basic_string<C>::size_type i=0;i<res.length();++i) {
			res[i] = upperCharacter(res[i]);
		}
		return res;
	}

	// ************************************************************************************
	template<typename C>
	std::basic_string<C> trimLeft(std::basic_string<C> str, const std::basic_string<C>& chars) {
		str.erase(0, str.find_first_not_of(chars));
		return str;
	}

	// ************************************************************************************
	template<typename C>
	std::basic_string<C> trimRight(std::basic_string<C> str, const std::basic_string<C>& chars) {
		str.erase(str.find_last_not_of(chars)+1);
		return str;
	}

	// ************************************************************************************
	template<typename C>
	std::basic_string<C> trim(const std::basic_string<C>& str, const std::basic_string<C>& chars) {
		return trimLeft<C>(trimRight<C>(str, chars), chars);
	}

	// ************************************************************************************
	template<typename C>
	bool endsWith(const std::basic_string<C>& str, const std::basic_string<C>& test) {
		if (str == test) return true;
		if (test.length() > str.length()) return false;
		return str.substr(str.length() - test.length(), test.length()) == test;
	}

	// ************************************************************************************
	template<typename C>
	bool startsWith(const std::basic_string<C>& str, const std::basic_string<C>& test) {
		if (str == test) return true;
		if (test.length() > str.length()) return false;
		return str.substr(0, test.length()) == test;
	}

	// ************************************************************************************
	template<typename C>
	std::basic_string<C> replace(const std::basic_string<C>& str, const std::basic_string<C>& search, const std::basic_string<C>& replacement) {
		std::basic_string<C> s = str;
		boost::replace_all(s, search, replacement);
		return s;
	}

	// ************************************************************************************
	std::string tabLines(const std::string& s, int32_t spaces);
	std::string simpleEncode(const std::string& str);
	std::string simpleDecode(const std::string& str);
	std::string convertCP1250ToUTF8(const std::string& str);
	std::string convertUTF8ToCP1250(const std::string& str);

	// ************************************************************************************
	template<typename C>
	std::vector<std::basic_string<C>> split(const std::basic_string<C>& str, const std::basic_string<C>& separators) {
		std::vector<std::basic_string<C>> splitted;
		boost::split(splitted, str, boost::is_any_of(separators));
		return splitted;
	}

	// ************************************************************************************
	template<typename C, typename CC>
	std::basic_string<CC> join(const C& arr, const std::basic_string<CC>& sep) {
		std::basic_stringstream<CC> ss;
		bool first = true;
		for(auto it=arr.begin();it != arr.end();++it) {
			if (!first) {
				ss << sep;
			}
			ss << *it;
			first = false;
		}
		return ss.str();
	}

	// ************************************************************************************
	template<typename C>
	void parseIntContainer(std::vector<C>& out, const std::string& value, const std::string& sep) {
		auto arr = split<char>(value,sep);
		for(auto& v: arr) {
			out.push_back(toInt(v));
		}
	}

	// ************************************************************************************
	template<typename C>
	void parseIntContainer(std::list<C>& out, const std::string& value, const std::string& sep) {
		auto arr = split<char>(value,sep);
		for(auto& v: arr) {
			out.push_back(toInt(v));
		}
	}

	// ************************************************************************************
	template<typename C>
	void parseIntContainer(std::set<C>& out, const std::string& value, const std::string& sep) {
		auto arr = split<char>(value,sep);
		for(auto& v: arr) {
			out.insert(toInt(v));
		}
	}


} /* namespace core */

#endif /* INCLUDE_STDEXT_STRING_H_ */
