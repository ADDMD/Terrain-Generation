// #include <iostream>
// #include <fmt/format.h>
// #include <stdio.h>
// #include "config.hpp"

// int main()
// {
// 	config c("../config.txt");
// 	if(not c.is_open()){
// 		fmt::print("Config file not opened\n");
// 		return EXIT_FAILURE;
// 	}

// 	int dim = std::stoi(c["dimension"]);
// 	std::string idx = c["index"];
// 	std::string val = c["value"];
// 	fmt::print("dimensione è {}\n", dim);
// 	fmt::print("index è {}\n", idx);
// 	fmt::print("value è {}\n", val);
// 	c.close();

// 	fmt::print("File is reopened? {}", c.open() ? "Yes\n" : "No\n");
// 	std::string newVal = "1";
// 	fmt::print("Inserting index with value {}\n", newVal);
// 	c.setAttribute("index", newVal);
// 	c.close();
// }