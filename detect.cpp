/* detect.cpp */

#include "rorschach.h"

/* This file is dedicated to detecting if files have been modified, added to
 * or deleted from the directories we are watching. */

int detect(string filename, umap<string, fileInfo> & fileMap,
 			umap<string, vector<rule>> & rules) {
	
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


int detectDelete(umap<string, fileInfo> & fileMap, umap<string, vector<rule>> & rules) {
    
    /* Loop through the fileMap and detect unvisited files */
	vector<string> filesDeleted;
    for (auto ent = fileMap.begin(); ent != fileMap.end(); ent++) {
        
        if (ent->second.visited == false) {
			debug(&ent << "," << &(*ent));
            log("Detected \"DELETE\" event on \"" << ent->first << "\"");
            execute(ent->first, "DELETE", rules);
			filesDeleted.push_back(ent->first);
        
        } else {
            // Reset visited flag if the entry has been visited
            ent->second.visited = false;
        }

    }
	/* Loop through files deleted and erase them from fileMap */
	for(auto ent : filesDeleted) {
		fileMap.erase(ent);
	}

    return EXIT_SUCCESS;
}
