#include "./Config.hpp"
#include "./Logger.hpp"

#include <fstream>
#include <iostream>

#include <fmt/format.h>

log::Level log::Logger::loggingLevel = log::Level::INFO;

log::Logger::Logger(const std::string className, std::vector<std::ostream*> outStreams)
	: className(className), outStreams(outStreams){}

log::Logger::Logger(const std::string className, std::ostream* outStream) 
	: Logger(className, std::vector<std::ostream*>{outStream}){}

log::Logger::Logger(const std::string className) 
	: Logger(className, &std::cout){}


void log::Logger::log(const Level lvl, std::string message) {
	if(lvl < loggingLevel) return;

	for (auto stram : outStreams) {
		(*stram) << fmt::format("[{:^5}] {}: {}\n", getName(lvl), className, message);
	}
}

log::Logger::~Logger() {
	for (auto stream : outStreams) {
		stream->flush();
		try {
			((std::fstream*) stream)->close();
		} catch (std::exception e) {
			fmt::print(e.what());
		}
	}
}