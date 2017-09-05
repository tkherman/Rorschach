/* main.cpp */

#include "rorschach.h"

#include <cstdlib>

#include <iostream>


/* Functions */

void usage(const string programName, int status) {
    cerr << "Usage: " << programName << " [options] ROOT" << endl;
    cerr << endl;
    cerr << "Options:" << endl;
    cerr << "   -h              Print this help message" << endl;
    cerr << "   -f RULES        Load rules from this file (default is rules)" << endl;
    cerr << "   -t SECONDS      Time between scans (default is 5 seconds)" << endl;

    exit(status);
}


/* Main Execution */

int main(int argc, char *argv[]) {
    
    /* Intialize parameters */
    string ruleFile = "rules";
    int frequency = 5;
    string root;

    /* Parse command line arguments */
    string programName = argv[0];

    if (argc < 2)
        usage(programName, 1);
    

    int argind = 1;

    while (argind < argc) {
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
            if (argind < argc) {
                frequency = atoi(argv[argind++]);
            } else {
                log("Error: -t requires another argument for time in seconds");
                usage(programName, 1);
            }

        } else {
            if (argind == argc) {
                root = arg;
            
            } else {
                log("Error: please only provide one root directory for scanning");
                usage(programName, 1);
            }

        }
    }

    cout << "rules: " << ruleFile << ", frequency: " << frequency << ", root: " << root << endl;
}
