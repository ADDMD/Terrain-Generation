#include "config.hpp"
#include <fmt/format.h>
#include <stdio.h>
#include <string>
#include <ios>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <fstream>

config::config(std::string path) {
	configFilePath = path;
	configFile.open(configFilePath);
}

std::string config::getAttribute(std::string attrName){
	// fmt::print("[{}] Searching for {}\n",this->getName() , attrName);
	// pulisci gli error flags
	configFile.clear();
	// ritorna all'inizio del file
	configFile.seekg(0);
	// Cerca per ogni linea del file
	for(std::string line; std::getline(configFile, line);){
		if(line.front() == '#'){
			continue;
		}
		// Cerca la posizione del separatore
		int pos = line.find(":");
		// Se non trovi il separatore c'è un problema 
		if(pos == -1) {
			fmt::print("Please check if ':' separate attribute name and value.\nRemember the sintax <attrName:attrValue>\n");
			return "";
		}
		std::string attrNameRetrieved = line.substr(0,pos);
		boost::erase_all(attrNameRetrieved, " ");
		// fmt::print("[{}] Found attribute {}\n", this->getName(), attrNameRetrieved);
		if(attrNameRetrieved.compare(attrName) == 0){
			// fmt::print("[{}] Found\n", this->getName());
			std::string attrValue = line.substr(pos+1,line.length());
			boost::erase_all(attrValue, " ");
			return attrValue;
		}
	}
	fmt::print("[{}] {} not found\n", this->getName(), attrName);
	return "";
}

std::string config::operator[](std::string attrName){
	return this->getAttribute(attrName);
}

std::string config::getName(){
	return "config";
}

bool config::is_open(){
	return configFile.is_open();
}

int config::open(){
	// if(configFilePath.empty()){
	// 	fmt::print("Config filepath needs to be passed in the constructor\n");
	// }
	configFile.open(configFilePath);
	return this->is_open();
}

void config::close(){
	configFile.close();
}

config::~config(){
	close();
}

// void config::updateAttribute(std::string attrName, std::string attrValue){

// }

//TODO: update attribute already present and add new one if not 
// void config::setAttribute(std::string attrName, std::string attrValue){
// 	// pulisci gli error flags
// 	configFile.clear();
// 	// ritorna all'inizio del file
// 	configFile.seekg(0);
// 	std::string toWrite = attrName;
// 	toWrite.append(" : ").append(attrValue);
	
// 	fmt::print("Da scrivere {}\n", toWrite);
	
// 	int startCurrentLine = 0;
// 	for(std::string line; std::getline(configFile, line);){
		
// 		int pos = line.find(":");
// 		if(pos == -1){
// 			fmt::print("Nothing wrote\nPlease check if ':' separate attribute name and value.\nRemember the sintax <attrName:attrValue>\n");
// 		}
		
// 		std::string attrNameRetrieved = line.substr(0,pos);
// 		boost::erase_all(attrNameRetrieved, " ");
// 		if(attrNameRetrieved.compare(attrName) == 0){
// 			// richiama updateAttribute

// 			// mi posiziono a inizio riga
// 			configFile.seekg(startCurrentLine);
// 			// scrivo la riga
// 			configFile << toWrite;

// 			int currentPos = configFile.tellg();
// 			char c;
// 			// prendo il prossimo carattere
// 			configFile.get(c);
// 			// fino a che è diverso da \n
// 			while(c != '\n'){
// 				fmt::print("Ho trovato {}\n", c);
// 				configFile.get(c);
// 			}
// 			// ho letto ora lo \n e torno indietro
// 			configFile.unget();
// 			// questa è la posizione prima dello \n
// 			int finalPos = configFile.tellg();
// 			fmt::print("Ho trovato accapo in {}\n", finalPos+1);

// 			// mi riposiziono subito dopo alla riga nuova che ho scritto
// 			configFile.seekg(currentPos);
// 			fmt::print("Mi sposto in {} dopo quello che ho scritto\n", currentPos);
// 			for (int i = currentPos; i < finalPos; i++)
// 			{
// 				fmt::print("Scrivo spazio bianco in {}\n", configFile.tellg());
// 				configFile.put(' ');
// 			}
// 			fmt::print("Scrivo spazio bianco in {}\n", configFile.tellg());
// 			configFile.put('\n');
// 			return;
// 		}
// 		startCurrentLine = configFile.tellg();
// 	}

// 	// se la fine del file -1 è lo /n allora scrivo dopo lo /n 
// 	// altrimenti scrivo \n e il toWrite
// 	// if (configFile.eof()){
// 	// 	fmt::print("[{}] Sono a fine file in pos {}\n", this->getName(), configFile.tellg());
// 	// 	configFile.unget();
// 	// 	fmt::print("[{}] Sono a fine file dopo essere tornato indietro? {}\n", this->getName(), configFile.eof() ? "Si" : "No");
// 	// 	if (configFile.unget().unget().get() == '\n'){
// 	// 		toWrite.append("\n");
// 	// 		configFile.write(toWrite.c_str(), toWrite.length());
// 	// 	}
// 	// }


// 	// non trovato, devo aggiungerlo alla fine del file
// 	configFile << toWrite.append("\n");
// }