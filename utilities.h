/* utilities.h */

#ifndef UTILITIES_H
#define UTILITIES_H


#include "rorschach.h"

umap<string, vector<rule>> loadRules(const string filepath);
bool isValid(string, string, string);
void msgExit(string);

#endif
