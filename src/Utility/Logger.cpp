#include "Logger.hpp"
#include <fstream>

Logger::Logger(std::string className, std::ostream out) : className(className), out(out){}

Logger::Logger(std::string className) : Logger(className, std::ofstream("../log.txt")){

}