#ifndef CONFIG_H
#define CONFIG_H 

#include <stdio.h>
#include <string>
#include <fstream>

class config
{
	std::string configFilePath;
	std::fstream configFile;
	

	std::string getName();
public:
	config(std::string configFilePath);
	// void setAttribute(std::string attrName, std::string attrValue);
	/// Get attribute value of the attribute whose name is passed.
	/// If no attribute found or file sintax not recognized, returns ""
	std::string getAttribute(std::string attrName);
	std::string operator[](std::string attrName);
	/// Open file whose path was passed in constructor
	int open();
	/// Check if the file has been opened
	bool is_open();
	/// Close the file
	void close();
	// ~config();
};


#endif