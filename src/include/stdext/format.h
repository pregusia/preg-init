/*
 * format.h
 *
 *  Created on: 9 wrz 2015
 *      Author: pregusia
 */

#ifndef INCLUDE_STDEXT_FORMAT_H_
#define INCLUDE_STDEXT_FORMAT_H_

#include <string>
#include <cstring>
#include <cassert>
#include <tuple>

namespace stdext {

	namespace internal {

		class SNPrintfCache {
			public:
				std::string values[16];
				int32_t n;

				SNPrintfCache() : n(0) { }
		};

		/*
		template<typename T>
		typename std::enable_if<std::is_integral<T>::value ||
								std::is_pointer<T>::value ||
								std::is_floating_point<T>::value ||
								std::is_enum<T>::value, T>::type sprintf_cast(SNPrintfCache& cache, const T& t) { return t; }
								*/

		inline int32_t sprintf_cast(SNPrintfCache& cache, const int32_t& v) { return v; }
		inline uint32_t sprintf_cast(SNPrintfCache& cache, const uint32_t& v) { return v; }
		inline int64_t sprintf_cast(SNPrintfCache& cache, const int64_t& v) { return v; }
		inline uint64_t sprintf_cast(SNPrintfCache& cache, const uint64_t& v) { return v; }
		inline float sprintf_cast(SNPrintfCache& cache, const float& v) { return v; }
		inline double sprintf_cast(SNPrintfCache& cache, const double& v) { return v; }
		inline uint8_t sprintf_cast(SNPrintfCache& cache, const uint8_t& v) { return v; }
		inline const char* sprintf_cast(SNPrintfCache& cache, const char *s) { return s; }

		template<typename T>
		inline const char* sprintf_cast(SNPrintfCache& cache, const T& v) {
			cache.n += 1;
			cache.values[cache.n - 1] = unsafeCast<std::string>(v);
			return cache.values[cache.n - 1].c_str();
		}

		template<int N> struct expand_snprintf {
			template<typename Tuple, typename... Args>
			static int call(SNPrintfCache& cache, char *s, size_t maxlen, const char *format, Tuple& tuple, const Args&... args) {
				return expand_snprintf<N-1>::call(cache, s, maxlen, format, tuple, sprintf_cast(cache, std::get<N-1>(tuple)), args...);
			}
		};

		template<> struct expand_snprintf<0> {
			template<typename Tuple, typename... Args>
			static int call(SNPrintfCache& cache, char *s, size_t maxlen, const char *format, Tuple& tuple, const Args&... args) {
				return snprintf(s, maxlen, format, args...);
			}
		};

		// Improved snprintf that accepts std::string and other types
		template<typename... Args>
		int snprintf(char *s, size_t maxlen, const char *format, const Args&... args) {
			SNPrintfCache cache;
			std::tuple<typename stdext::replace_extent<Args>::type...> tuple(args...);
			return expand_snprintf<std::tuple_size<decltype(tuple)>::value>::call(cache, s, maxlen, format, tuple);
		}

		template<typename... Args>
		inline int snprintf(char *s, size_t maxlen, const char *format) {
			std::strncpy(s, format, maxlen);
			s[maxlen-1] = 0;
			return strlen(s);
		}
	}

	// ************************************************************************************
	template<typename... Args>
	static inline std::string format() { return std::string(); }

	// ************************************************************************************
	template<typename... Args>
	static inline std::string format(const std::string& format) { return format; }

	// ************************************************************************************
	template<typename... Args>
	static std::string format(const std::string& format, const Args&... args) {
		int n, size = 1024;
		std::string str;
		while(true) {
			str.resize(size);
			n = internal::snprintf(&str[0], size, format.c_str(), args...);
			assert(n != -1);
			if(n < size) {
				str.resize(n);
				return str;
			}
			size *= 2;
		}
	}

}



#endif /* INCLUDE_STDEXT_FORMAT_H_ */
