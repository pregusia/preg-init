#ifndef LOGGER_H
#define LOGGER_H

#include <base.h>
#include <fstream>
#include <mutex>
#include "exceptions.h"

namespace core {

	ENUM_DEFINE(LogLevel,
		Debug = 1,
		Info = 2,
		Script = 3,
		Warning = 4,
		Error = 5,
		Fatal = 6,
	);


	class Logger
	{
	    enum {
	    	MAX_LOG_HISTORY = 1000
	    };

	public:
	    void log(LogLevel::Enum level, const std::string& message, bool dumpTrace);

	    void debug(const std::string& what, bool dumpTrace=false) { log(LogLevel::Debug, what, dumpTrace); }
	    void info(const std::string& what, bool dumpTrace=false) { log(LogLevel::Info, what, dumpTrace); }
	    void script(const std::string& what, bool dumpTrace=false) { log(LogLevel::Script, what, dumpTrace); }
	    void warning(const std::string& what, bool dumpTrace=false) { log(LogLevel::Warning, what, dumpTrace); }
	    void error(const std::string& what, bool dumpTrace=false) { log(LogLevel::Error, what, dumpTrace); }
	    void fatal(const std::string& what, bool dumpTrace=false) { log(LogLevel::Fatal, what, dumpTrace); }
	    void exception(const std::string& msg, const Exception& e);

	    void setLogFile(const std::string& file);

	private:
	    std::ofstream m_outFile;
	    std::recursive_mutex m_mutex;
	};

}

extern core::Logger g_logger;

#endif
