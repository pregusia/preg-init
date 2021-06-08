#include "logger.h"

core::Logger g_logger;

namespace core {

	// ************************************************************************************
	void Logger::exception(const std::string& msg, const core::Exception& e) {
		std::stringstream ss;
		ss << msg << std::endl;
		ss << "message=" << e.message() << std::endl;
		ss << "trace:";
		ss << stdext::tabLines(e.trace(),3);
		error(ss.str(), false);
	}

	// ************************************************************************************
	void Logger::log(LogLevel::Enum level, const std::string& message, bool dumpTrace)
	{
	    std::lock_guard<std::recursive_mutex> lock(m_mutex);
	    static bool ignoreLogs = false;
	    if (ignoreLogs) return;

	    // prepare final message
	    StringVector vec;
	    std::string ts = stdext::Time::asStringWithMilis();
	    vec = stdext::split<char>(message,"\n");

	    // outputing
	    for(auto s: vec) {
		    std::stringstream buf;
		    buf << "[" << ts << "] ";

		    switch(level) {
		    	case LogLevel::Debug:   buf << "[DEBUG ] "; break;
		    	case LogLevel::Error:   buf << "[ERROR ] "; break;
		    	case LogLevel::Fatal:   buf << "[FATAL ] "; break;
		    	case LogLevel::Info:    buf << "[ INFO ] "; break;
		    	case LogLevel::Script:  buf << "[SCRIPT] "; break;
		    	case LogLevel::Warning: buf << "[ WARN ] "; break;
		    }

		    buf << s;

		    std::cerr << buf.str() << std::endl;
		    if(m_outFile.good()) {
				m_outFile << buf.str() << std::endl;
				m_outFile.flush();
		    }
	    }

	    if(level == LogLevel::Fatal) {
	    	//platformFatalError(message);
			ignoreLogs = true;
			exit(-1);
	    }
	}

	void Logger::setLogFile(const std::string& file)
	{
	    std::lock_guard<std::recursive_mutex> lock(m_mutex);

	    m_outFile.open(file.c_str(), std::ios::out | std::ios::app);
	    if(!m_outFile.is_open() || !m_outFile.good()) {
	    	g_logger.error(stdext::format("Unable to save log to '%s'", file));
	    	return;
	    }
	    m_outFile.flush();
	}
	
}

