/* utilities.cpp */

/* This file includes the implementations for utility functions crucial for
	rorscach to work effectively */

#include "rorschach.h"

#include <fstream>
#include <sstream>




/* This function parse the rules in the given file and store them in a
 * unordered_map which is return to main() 
 * If any of the rules are invalid, it will return an empty unordered_map
 * so the main function will know the exit program*/
umap<string, vector<rule>> loadRules(const string filepath) {
    
    /* Initialize the unordered_map */
	umap<string, vector<rule>> ruleMap;
    ruleMap["CREATE"] = vector<rule> ();
    ruleMap["MODIFY"] = vector<rule> ();
    ruleMap["DELETE"] = vector<rule> ();

	
    /* Open the file and begin parsing theh rules */
	ifstream file(filepath);
	string line; //for reading in file line by line
	while(getline(file, line)) {

		if(line.empty()) continue;
		
		/* Declare variables */
        string event;
        string pattern;
        string action;
        
        /* Parse file line by lin */
		istringstream iss(line);
		iss >> event;
		

		if(event[0] == '#') continue; // skip lines beginning with '#'
		
        
        if (!iss.eof()) iss >> pattern;
        else {
            log("Error: Invalid pattern in rules");
            return ruleMap.clear();
        }
		

		// Fill action
        if (iss.eof()) {
            log("Error: No action in rules");
            return ruleMap.clear();
        
        } else {
            string filler;
            iss >> filler;
            action = filler;
            
            while (!iss.eof()) {
                iss >> filler;
                action += " " + filler;
            }
        }
        
        
        /* Add the rule to map if it is valid */
        rule myRule(pattern, action);
	    vector<rule>::iterator eventVecIT = ruleMap.find(event)

        if (eventVecIT != ruleMap.end())
            (*eventVecIT).push_back(myRule);
        else {
            log("Error: Invalid event in rules")
            return ruleMap.clear();
        }
    }

	return ruleMap;
}
