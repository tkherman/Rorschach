/* execute.cpp */

/* This file contains the function that carries out the execution of an action
 * when a event happens and the filename matches a pattern */

#include "rorschach.h"

#include <unistd.h>

/* This function finds out if the filename matches any pattern and event in rules */
string



/* This function take in a filename, a event and rules then determine if an action
 * should be execute according to the filename, event, and rules. If so, fork a
 * process to execute the action. The child process should run concurrently so
 * the parent should ignore children */
int execute(const string filename, const string event, const umap<string, vector<rule>> & rules) {
    
}
