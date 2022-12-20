#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <fmt/format.h>

namespace log {
	enum class Level{
		TRACE = 0,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NONE
	};

	class Logger {

	protected:
		std::string className;
		std::vector<std::ostream*> outStreams;

	public:
		static Level loggingLevel;

		Logger(const std::string className);
		Logger(const std::string className, std::ostream* outStream);
		Logger(const std::string className, std::vector<std::ostream*> outStreams);
		~Logger();
		void log(const Level lvl, std::string message);
		
		
	};


	inline std::string getName(Level lvl) {
		std::string name = "";
		switch(lvl) {
		case Level::TRACE:
			name = "TRACE";
			break;
		case Level::DEBUG:
			name = "DEBUG";
			break;
		case Level::INFO:
			name = "INFO";
			break;
		case Level::WARN:
			name = "WARN";
			break;
		case Level::ERROR:
			name = "ERROR";
			break;
		case Level::FATAL:
			name = "FATAL";
			break;
		case Level::NONE:
			name = "NONE";
			break;
		}
		return name;
	}

	inline Level getLevel(std::string name) {

		if(name == "TRACE") {
			return Level::TRACE;
		}
		if(name == "DEBUG") {
			return Level::DEBUG;
		}
		if(name == "INFO") {
			return Level::INFO;
		}
		if(name == "WARN") {
			return Level::WARN;
		}
		if(name == "ERROR") {
			return Level::ERROR;
		}
		if(name == "FATAL") {
			return Level::FATAL;
		}
		if(name == "NONE") {
			return Level::NONE;
		}
		return Level::TRACE;
	}

	inline void setLoggingLevel(Level level){
		Logger::loggingLevel = level;
	}



}

#endif