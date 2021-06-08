/*
 * enums.cpp
 *
 *  Created on: 3 gru 2015
 *      Author: pregusia
 */

#include "enums.h"
#include "string.h"

namespace stdext {

	// ************************************************************************************
	std::vector<std::string> EnumHelper::parseNames(const char* nnn) {
		std::vector<std::string> res;

		auto names = stdext::split<char>(nnn, ",");
		int32_t i = 0;
		for(auto& s: names) {
			auto arr = stdext::split<char>(s, "=");
			if (arr.empty()) continue;
			auto n = stdext::trim<char>(arr[0]," ");
			if (!n.empty()) {
				res.push_back(n);
			}
		}

		return res;
	}

	// ************************************************************************************
	void EnumHelper::countValues(int32_t& n, const char* names) {
		if (n == -1) {
			auto arr = parseNames(names);
			n = arr.size();
		}
	}

}
