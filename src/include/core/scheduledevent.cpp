#include "scheduledevent.h"

namespace core {

	// ************************************************************************************
	ScheduledEvent::ScheduledEvent(const std::function<void()>& callback, int delay, int maxCycles) : Event(callback) {
	    m_ticks = g_clock.millis() + delay;
	    m_delay = delay;
	    m_maxCycles = maxCycles;
	    m_cyclesExecuted = 0;
	}

	// ************************************************************************************
	void ScheduledEvent::execute() {
	    if(!m_canceled && m_callback && (m_maxCycles == 0 || m_cyclesExecuted < m_maxCycles)) {
	    	m_callback();
	    	m_executed = true;
	    	// callback may be used in the next cycle
	    } else {
	    	// reset callback to free object refs
	    	m_callback = nullptr;
	    }

	    m_cyclesExecuted++;
	}

	// ************************************************************************************
	bool ScheduledEvent::nextCycle() {
	    if(m_callback && !m_canceled && (m_maxCycles == 0 || m_cyclesExecuted < m_maxCycles)) {
	    	m_ticks += m_delay;
	    	return true;
	    }

	    // reset callback to free object refs
	    m_callback = nullptr;
	    return false;
	}
	
}

