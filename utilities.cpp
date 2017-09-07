/* utilities.cpp */

#include "rorschach.h"

#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>

/* Retrieve the modification time of the given file */
time_t getMTime(const string filename) {
    struct stat s;
    int status = lstat(filename.c_str(), &s);
    
    // lstat should returns 0 if successful
    if (!status) return s.st_mtime;
    else {
        log("Error: " << strerror(errno));
        exit(EXIT_FAILURE);
    
    }
}
