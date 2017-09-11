/* detect.cpp */

#include "rorschach.h"

/* This file is dedicated to detecting if files have been modified, added to
 * or deleted from the directories we are watching. */

int detect(const string filename, umap<string, fileInfo> & fileMap,
 			const umap<string, vector<rule>> & rules) {
	
	time_t modTime = getMTime(filename);
	
    /* Determine if a file has been modified */
	if (fileMap.find(filename) != fileMap.end()) { //if file is in map
		
        if(fileMap[filename].mtime != modTime) { //modified
			fileMap[filename].mtime = modTime;
			
            log("Detected \"MODIFY\" event on \"" << filename << "\"");
			execute(filename, "MODIFY", rules);
		}
		fileMap[filename].visited = true;
    
    /* Determine if a file has been created */
	} else {
		fileInfo entry = fileInfo(modTime, true);
		fileMap[filename] = entry;
		
		log("Detected \"CREATE\" event on \"" << filename << "\"");
		execute(filename, "CREATE", rules);
	}

	return EXIT_SUCCESS;
}


int detectDelete(umap<string, fileInfo> & fileMap, const umap<string, vector<rule>> & rules) {
    
    /* Loop through the fileMap and detect unvisited files */
    for (auto ent = fileMap.begin(); ent != fileMap.end(); ent++) {
        
        if (ent->second.visited == false) {
            //execute(ent->first, "DELETE", rules);
            log("Detected \"DELETE\" event on \"" << ent->first << "\"");
            execute(ent->first, "DELETE", rules);
            fileMap.erase(ent);
        
        } else {
            // Reset visited flag if the entry has been visited
            ent->second.visited = false;
        }

    }

    return EXIT_SUCCESS;
}