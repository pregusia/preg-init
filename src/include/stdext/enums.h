/*
 * enums.h
 *
 *  Created on: 3 gru 2015
 *      Author: pregusia
 */

#ifndef INCLUDE_STDEXT_ENUMS_H_
#define INCLUDE_STDEXT_ENUMS_H_

#include <string>
#include <vector>

namespace stdext {

	class EnumHelper {
		private:
			EnumHelper() { }

		public:

			static std::vector<std::string> parseNames(const char* names);
			static void countValues(int32_t& n, const char* names);

	};

} /* namespace stdext */

#endif /* INCLUDE_STDEXT_ENUMS_H_ */
