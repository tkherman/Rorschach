/* utilities.cpp */

/* This file includes the implementations for utility functions crucial for
	rorscach to work effectively */

#include "utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>

umap<string, vector<rule>> loadRules(const string filepath);
bool isValid(string, string, string);
void msgExit(string);

umap<string, vector<rule>> loadRules(const string filepath) {

	umap<string, vector<rule>> ruleMap;
	
	ifstream file(filepath);
	string line; //for reading in file line by line
	while(getline(file, line)) {

		//if string is empty, skip
		if(line.empty()) continue;
		
		//parse line
		istringstream iss(line);
		string event, pattern;
		iss >> event;
		
		//if string begins with '#', skip
		if(event[0] == '#') continue;
		
		if(!(iss>> pattern)) {
			msgExit("Error: Wrong Number of Rules");
		}
		
		//begin filling action string and check for error
		string action, filler;
		if(!(iss >> filler)) 
			msgExit("Error: Wrong Number of Rules");
		else 
			action += filler + " ";
		
		//continue filling the action string (could have spaces, so need loop)
		while(iss >> filler) action += filler + " ";

		//add to map if valid, exit if not
		if(isValid(event, pattern, action)) {
			rule myRule = {pattern, action};
			ruleMap[event].push_back(myRule);
		} else { 
			msgExit("Error: Invalid Rule Format");
		}
		
	}

	return ruleMap;
}

/* This function prints an error message before calling exit() */
void msgExit(string msg) {
	cerr << msg << endl;
	exit(EXIT_FAILURE);
}

/* This function determines if the inputs are valid. */
bool isValid(string event, string pattern, string action) {
	if(event != "MODIFY" && event != "DELETE" && event != "CREATE")
		return false;
	
	//TODO
	//can't think of any reason pattern or action would be invalid as of now
	//if still cannot later, then remove pattern and action from function
	
	return true;
}

