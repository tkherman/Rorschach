/* execute.cpp */

/* This file contains the function that carries out the execution of an action
 * when a event happens and the filename matches a pattern */

#include "rorschach.h"

#include <ctime>
#include <unistd.h>
#include <libgen.h>
#include <fnmatch.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stringstream>


/* This function determines if an action should be executed based on the rules.
 * If filename matches a pattern, the function returns the action
 * else, the function returns an empty string */
string determineAction(const string filename, const string event, const umap<string, vector<rule>> & rules) {
    
    /* Loop thorugh vector of corresponding event and check if there's matching pattern */
    for (auto it = rules[event].begin(); it != rules[event].end(); it++) {
        if (!fnmatch(it->pattern.c_str(), filename.c_str()))
            return it->action;
    }

    return "";
}



/* This function take in a filename, a event and rules then determine if an action
 * should be execute according to the filename, event, and rules. If so, fork a
 * process to execute the action. */
int execute(const string filename, const string event, const umap<string, vector<rule>> & rules) {
    
    string action = determineAction(filename, event, rules);
    
    /* The filename matches pattern, fork process to execute action */
    if (!action.empty()) {
        pid_t id = fork();

        if (id > 0) { // parent process
            int status;
            wait(&status);

            return EXIT_SUCCESS;
        
        } else if (id == 0) { // child process
            
            /* set environment variables */
            setenv("FULLPATH", filename.c_str(), 1);
            setenv("BASEPATH", basename(filename.c_str()), 1);
            setenv("EVENT", event.c_str(), 1);
            
            time_t seconds;
            time(&seconds)
            string strTime = to_string(seconds);
            setenv("TIMESTAMP", strTime.c_str(), 1);
            
            /* Parse the action command into a char*[] from string */
            stringstream ssin(action);
            vector<string> actionParser;
            string temp;
            
            // split string by space and put them into a vector
            ssin >> temp;
            while (!ssin.eof()) {
                actionParser.push_back(temp);
                ssin>> temp;
            }

            // convert strings into c_strings and put them into a char*[]
            int execArgc = actionParser.length();
            char *execArgv[execArgc + 1];
            for (int i = 0; i < execArgc; i++)
                execArgv[i] = actionParser[i].c_str();

            execArgv[execArgc] = NULL;

            // execute the command
            if (execvp(execArgv[0], execArgv) < 0)
                    log("Error: unable to exec " << strerror(errno));

            _exit(EXIT_FAILURE);

        } else {
            log("Error: fork failed " << strerror(errno));
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
