#include "./Config.hpp"
#include "./Logger.hpp"

#include <fstream>
#include <iostream>

#include <fmt/format.h>

logtg::Level logtg::Logger::loggingLevel = logtg::Level::INFO;

logtg::Logger::Logger(const std::string className, std::vector<std::ostream*> outStreams)
	: className(className), outStreams(outStreams){}

logtg::Logger::Logger(const std::string className, std::ostream* outStream) 
	: Logger(className, std::vector<std::ostream*>{outStream}){}

logtg::Logger::Logger(const std::string className) 
	: Logger(className, &std::cout){}


void logtg::Logger::log(const Level lvl, std::string message) {
	if(lvl < loggingLevel) return;

	for (auto stram : outStreams) {
		(*stram) << fmt::format("[{:^5}] {}: {}\n", getName(lvl), className, message);
	}
}

logtg::Logger::~Logger() {
	for (auto stream : outStreams) {
		stream->flush();
		try {
			((std::fstream*) stream)->close();
		} catch (std::exception e) {
			fmt::print(e.what());
		}
	}
}