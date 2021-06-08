#include "event.h"

namespace core {

	// ************************************************************************************
	Event::Event(const std::function<void()>& callback) :
	    m_callback(callback),
	    m_canceled(false),
	    m_executed(false)
	{
	}

	// ************************************************************************************
	Event::~Event() {
	    // assure that we lost callback refs
	    //assert(m_callback == nullptr);
	}

	// ************************************************************************************
	void Event::execute() {
	    if(!m_canceled && !m_executed && m_callback) {
	    	m_callback();
	    	m_executed = true;
	    }

	    // reset callback to free object refs
	    m_callback = nullptr;
	}

	// ************************************************************************************
	void Event::cancel() {
	    m_canceled = true;
	    m_callback = nullptr;
	}
	
}

