#include "clock.h"

core::Clock g_clock;

namespace core {

	// ************************************************************************************
	Clock::Clock() {
	    m_currentMicros = 0;
	    m_currentMillis = 0;
	    m_currentSeconds = 0;
	    m_unixTime = 0;
	}

	// ************************************************************************************
	void Clock::update() {
	    m_currentMicros = stdext::Time::micros();
	    m_currentMillis = m_currentMicros / 1000;
	    m_currentSeconds = m_currentMicros / 1000000.0f;
	    m_unixTime = stdext::Time::seconds();
	}
	
	// ************************************************************************************
	WallTime Clock::utcTime() {
		time_t now = ::time(nullptr);
		auto tm = gmtime(&now);

		WallTime res;
		res.day = tm->tm_mday;
		res.hours = tm->tm_hour;
		res.minutes = tm->tm_min;
		res.month = tm->tm_mon;
		res.seconds = tm->tm_sec;
		res.year = 1900 + tm->tm_year;
		return res;
	}

	// ************************************************************************************
	WallTime Clock::localTime() {
		time_t now = ::time(nullptr);
		auto tm = localtime(&now);

		WallTime res;
		res.day = tm->tm_mday;
		res.hours = tm->tm_hour;
		res.minutes = tm->tm_min;
		res.month = tm->tm_mon;
		res.seconds = tm->tm_sec;
		res.year = 1900 + tm->tm_year;
		return res;
	}




	// ************************************************************************************
	ticks_t ClockTimer::millisElapsed() const {
		return (g_clock.micros() - m_start) / 1000;
	}

	// ************************************************************************************
	ticks_t ClockTimer::microsElapsed() const {
		return g_clock.micros() - m_start;
	}

	// ************************************************************************************
	float ClockTimer::secondsElapsed() const {
		return (float)(g_clock.micros() - m_start) / 1000000.0f;
	}

	// ************************************************************************************
	void ClockTimer::restart() {
		m_start = g_clock.micros();
	}



}

