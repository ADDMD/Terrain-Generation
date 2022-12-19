#include <string>
#include <ostream>

class Logger
{
	enum Level{
		TRACE = 0,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL
	};
	std::string className;
	std::ostream out;
public:
	Logger(std::string className);
	Logger(std::string className, std::ostream out);
	void log(std::string message, Level lvl);
	~Logger();
	
};