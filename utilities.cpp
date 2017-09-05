/* utilities.cpp */

/* This file includes the implementations for utility functions crucial for
	rorscach to work effectively */

#include "utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>

umap<string, rule> loadRules(string filepath) {
	
	ifstream file(filepath);
	string line; //for reading in file line by line
	while(getline(file, line)) {
		cout << line << endl;
		
	}

	umap<string, rule> test;
	return test;
}

int main() {
	umap<string, rule> test = loadRules("rules.txt");
	return 0;
}
