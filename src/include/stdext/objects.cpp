/*
 * objects.cpp
 *
 *  Created on: 25 mar 2016
 *      Author: pregusia
 */

#include "stdext.h"

namespace stdext {

	// ************************************************************************************
	object::object() {
		static uint64_t counter = 100;
		m_objectRefs = 0;
		m_objectId = counter++;
	}

	// ************************************************************************************
	object::~object() {
		assert(m_objectRefs == 0);
	}

}



