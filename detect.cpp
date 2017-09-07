/* detect.cpp */

#include "rorschach.h"

/* This file is dedicated to detecting if files have been modified or added to
 * the directories we are watching. */

int detect(const string filename, umap<string, fileInfo> & fileMap,
 			const umap<string, vector<rule>> & rules) {
	
	time_t modTime = getMTime(filename);
	
	if (fileMap.find(filename) != fileMap.end()) { //if file is in map
		//check if modified
		if(fileMap[filename].mtime != modTime) { //modified
			//remark & notify
			fileMap[filename].mtime = modTime;
			printf("Detected \"MODIFY\" event on \"%s\"\n", filename.c_str());
			//exec
			//execute(filename, "MODIFY", rules);
		}
		fileMap[filename].visited = true;
	} else {
		//add to map
		fileInfo entry = fileInfo(modTime, true);
		fileMap[filename] = entry;
		//note event
		printf("Detected \"CREATE\" event on \"%s\"\n", filename.c_str());
		//exec
		//execute(filename, "MODIFY", rules);
	}

	return EXIT_SUCCESS;
}
