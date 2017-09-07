/* scan.cpp */

/* This file contails the main fuctions that carries out the scanning of the
 * directory to be monitored */

#include "rorschach.h"

#include <dirent.h>

#include <string>

/* This functions carries out the actual recursive scan, it takes in an additional
 * boolean argument firstScan to differentiate if this is a repeating scan or
 * first scan */
int recursiveScan(const string root, const umap<string, vector<rules>> & rules, 
                    umap<string, fileInfo> & fileMap, bool firstScan) {
    struct dirent *dp;
    DIR *dir = opendir(root.c_str());

    if (dir == NULL) {
        log("Error:")
    }
}



int scan(const string root, int frequency, const umap<string, vector<rules>> & rules) {
    
    /* Initialize fileMap */
    umap<string, fileInfo> fileMap;

    /* Carry out the initial scan */
    int status = recursiveScan(root, rules, fileMap, true);
}
