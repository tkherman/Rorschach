/* scan.cpp */

/* This file contains the main fuctions that carries out the scanning of the
 * directory to be monitored */

#include "rorschach.h"

#include <dirent.h>
#include <unistd.h>
#include <cstdlib>

#include <string>

/* This functions carries out the actual recursive scan, it takes in an additional
 * boolean argument firstScan to differentiate if this is a repeating scan or
 * first scan */
int recursiveScan(const string root, const umap<string, vector<rule>> & rules, 
                    umap<string, fileInfo> & fileMap, bool firstScan) {
    
    
    /* Open directory */
    struct dirent *dp;
    DIR *dir = opendir(root.c_str());


    /* Loop through contents of the directory, recurse on directories within */
    if (dir) {
        while ((dp = readdir(dir)) != NULL) {
            // Skip "." and ".."
            if (streq(dp->d_name, ".") || streq(dp->d_name, "..")) continue;

            
            string filename = root + "/" + string(dp->d_name);
            //debug(filename);

            /* If it's first scan, simply add filename to umap */
            if (firstScan) {
                if (dp->d_type == DT_DIR) {
                    recursiveScan(filename, rules, fileMap, true);
                
                } else {
                    fileInfo info(getMTime(filename), false);
                    fileMap[filename] = info;
                }
            /* Repeating scan, call detect for all file while recursing on directories */
            } else {
                if (dp->d_type == DT_DIR) {
                    recursiveScan(filename, rules, fileMap, false);
                //log("Scanning");
				}else {
                    detect(filename, fileMap, rules);
                }
            }
        }
    } else {
		debug("failed to open directory");
		fprintf(stderr, "Error: Failed to open directory\n");
		exit(EXIT_FAILURE);
	}

	closedir(dir);
	return 0;
}



int scan(const string root, int frequency, const umap<string, vector<rule>> & rules) {
    
    /* Initialize fileMap */
    umap<string, fileInfo> fileMap;

    /* Carry out the initial scan */
    log("Carrying out initial scan...");
    int status = recursiveScan(root, rules, fileMap, true);
    log("Initial scan completed.")

    
    /* Begin repeating scan perodically */
    log("Beginning perodic scan:");
    log("   scanning root directory every " << frequency << " seconds");
    while (true) {
		usleep(5*1000000);
		debug("begin of scan");
        recursiveScan(root, rules, fileMap, false);

        /* Detect if there's any deletion */
        for (auto ent : fileMap) {
            if (ent->second.visited == false) {
                execute(ent->first, "DELETE", rules);
                fileMap.erase(ent);
            }
        }
    }
}
