#ifndef CLOCK_H
#define CLOCK_H

#include <base.h>

namespace core {

	class WallTime {
		public:
			int32_t seconds;
			int32_t minutes;
			int32_t hours;
			int32_t day;
			int32_t month;
			int32_t year;

			WallTime() {
				seconds = 0;
				minutes = 0;
				hours = 0;
				day = 0;
				month = 0;
				year = 0;
			}
	};

	class Clock {
		public:
			Clock();

			void update();

			ticks_t micros() { return m_currentMicros; }
			ticks_t millis() { return m_currentMillis; }
			float seconds() { return m_currentSeconds; }
			ticks_t time() { return m_unixTime; }
			int32_t normalizedTime() { return m_unixTime - 1468589110UL; }

			WallTime utcTime();
			WallTime localTime();

		private:
			ticks_t m_currentMicros;
			ticks_t m_currentMillis;
			float m_currentSeconds;
			ticks_t m_unixTime;
	};

	class ClockTimer {
		public:
			ClockTimer() { restart(); }

			ticks_t millisElapsed() const;
			ticks_t microsElapsed() const;
			float secondsElapsed() const;

			void restart();

		private:
			ticks_t m_start;
	};

}

extern core::Clock g_clock;

#endif

