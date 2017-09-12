/* loadRules.cpp */

/* This file includes the implementations for utility functions crucial for
	rorscach to work effectively */

#include "rorschach.h"

#include <fstream>
#include <sstream>



/* This function parse the rules in the given file and store them in a
 * unordered_map which is return to main()
 * If any of the rules are invalid, it exit the program*/
umap<string, vector<rule>> loadRules(const string filepath) {
    
    /* Initialize the unordered_map */
	umap<string, vector<rule>> ruleMap;
    ruleMap["CREATE"] = vector<rule> ();
    ruleMap["MODIFY"] = vector<rule> ();
    ruleMap["DELETE"] = vector<rule> ();
    
    log("Loading rules from file...");
	
    /* Opens file containing rules */
	ifstream file(filepath, ifstream::in);
    if (!file.is_open()) {
        log("Error: Fails to open rule file");
        exit(EXIT_FAILURE);
    }


	string line; //for reading in file line by line
	while(getline(file, line)) {

		if(line.empty()) continue;
		
		/* Declare variables */
        string event;
        string pattern;
        string action;
        
        /* Parse file line by line */
		istringstream iss(line);
		iss >> event;
		

		if(event[0] == '#') continue; // skip lines beginning with '#'
		
        
		if(!(iss>> pattern)) {
            log("Error: Invalid rule format");
            exit(EXIT_FAILURE);
		}
		

		/* Fill action */
		string filler;
        if(!(iss >> filler)) {
           log("Error: Invalid rule format");
           exit(EXIT_FAILURE);
		} else { 
			action = filler;
        }
		
		// Continue filling the action string (could have spaces, so need loop)
		while(iss >> filler) action += " " + filler;



		
        /* Add the rule to map if it is valid */
        rule myRule(pattern, action);
        umap<string, vector<rule>>::iterator eventVecIT = ruleMap.find(event);
        
        if (eventVecIT != ruleMap.end()) {
            eventVecIT->second.push_back(myRule);
        } else {
            log("Error: Invalid event in rules");
            exit(EXIT_FAILURE);
        }
		
	}
    
    log("Rules are successfully parsed and loaded from file");

	return ruleMap;
}
