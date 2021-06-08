/*
 * exceptions.h
 *
 *  Created on: 8 wrz 2015
 *      Author: pregusia
 */

#ifndef INCLUDE_CORE_EXCEPTIONS_H_
#define INCLUDE_CORE_EXCEPTIONS_H_

#include <base.h>
#include <exception>

namespace core {

	class Exception: public std::exception {
		public:
			Exception();
			Exception(const std::string& msg);
			virtual ~Exception() throw() { };

			virtual const char* what() const throw() { return m_message.c_str(); }
			const std::string& message() const { return m_message; }
			const std::string& trace() const { return m_trace; }

		protected:
			std::string m_message;
			std::string m_trace;
	};

	class CoreException: public Exception {
		public:
			CoreException(const std::string& msg) : Exception(msg) { }
			virtual ~CoreException() throw() { }
	};


} /* namespace core */

#endif /* INCLUDE_CORE_EXCEPTIONS_H_ */
