/*
 * time.h
 *
 *  Created on: 8 wrz 2015
 *      Author: pregusia
 */

#ifndef INCLUDE_STDEXT_TIME_H_
#define INCLUDE_STDEXT_TIME_H_

#include "types.h"
#include <string>

namespace stdext {

	class Time {
		private:
			Time() { }

		public:
			static ticks_t seconds();
			static ticks_t millis();
			static ticks_t micros();

			static void millisleep(size_t ms);
			static void microsleep(size_t us);

			static std::string asString(const char* format=nullptr);
			static std::string asStringWithMilis();
	};

	class RawTimer {
		public:
			RawTimer() { restart(); }

			ticks_t millisElapsed() const { return (Time::micros() - m_start) / 1000; }
			ticks_t microsElapsed() const { return Time::micros() - m_start; }
			float secondsElapsed() const { return (float)(Time::micros() - m_start) / 1000000.0f; }

			void restart() { m_start = Time::micros(); }

		private:
			ticks_t m_start;
	};

} /* namespace core */

#endif /* INCLUDE_CORE_TIME_H_ */
