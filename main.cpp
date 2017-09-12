/* main.cpp */

#include "rorschach.h"

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <csignal>
#include <iostream>


/* Functions */

void usage(string programName, int status) {
    cerr << endl;
    cerr << "Usage: " << programName << " [options] ROOT" << endl;
    cerr << endl;
    cerr << "Options:" << endl;
    cerr << "   -h              Print this help message" << endl;
    cerr << "   -f RULES        Load rules from this file (default is rules)" << endl;
    cerr << "   -t SECONDS      Time between scans (default is 5 seconds)" << endl;

    exit(status);
}

bool isNumber(char* s) {
    for (size_t i = 0; i < strlen(s); i++)
        if (!isdigit(s[i])) return false;

    return true;
}

void signalHandler(int signum) {
	cout << "\nCleaning up\nBye!\n";
	exit(EXIT_SUCCESS);
}


/* Main Execution */

int main(int argc, char *argv[]) {

	/* catch SIGINT */
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
    
    /* Intialize parameters */
    string ruleFile = "rules";
    int frequency = 5;
    string root;


    /* Parse command line arguments */

    string programName = argv[0];

    if (argc < 2)
        usage(programName, 1);
    

    int argind = 1;

    while (argind < argc - 1) {
        string arg = argv[argind++];

        if (!arg.compare("-h")) {
            usage(programName, 0);
        
        } else if (!arg.compare("-f")) {
            if (argind < argc) {
                ruleFile = argv[argind++];
            } else {
                log("Error: -f requires a filename for the rules");
                usage(programName, 1);
            }

        } else if (!arg.compare("-t")) {
            if ((argind < argc) && isNumber(argv[argind])) {
                frequency = atoi(argv[argind++]);
            
            } else if ((argind < argc) && !isNumber(argv[argind])) {
                log("Error: -t requires an integer as argument");
                usage(programName, 1);
            
            } else {
                log("Error: -t requires another argument for time in seconds");
                usage(programName, 1);
            }

        } else {
            log("Error: invalid argument " << arg)
            usage(programName, 1);

        }
    }
    

    /* Get the root directory */

    if ((argind + 1 == argc) && argv[argind][0] != '-') {
        root = argv[argind];
    
    } else {
        log("Error: please provide a root directory for scanning");
        usage(programName, 1);

    }


    /* Parse rules file and begin scanning */

    umap<string, vector<rule>> rules = loadRules(ruleFile);

    scan(root, frequency, rules);
    
}
