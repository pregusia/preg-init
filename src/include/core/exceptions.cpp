/*
 * exceptions.cpp
 *
 *  Created on: 8 wrz 2015
 *      Author: pregusia
 */

#include <base.h>
#include "exceptions.h"

namespace core {

	// ************************************************************************************
	Exception::Exception() {
		m_trace = "";
	}

	// ************************************************************************************
	Exception::Exception(const std::string& msg) : m_message(msg) {
		m_trace = "";
	}

} /* namespace core */
