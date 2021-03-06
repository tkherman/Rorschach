/* rorschach.h */

#ifndef RORSCHACH_H
#define RORSCHACH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <dirent.h>
#include <ctime>
#include <unistd.h>
#include <libgen.h>
#include <fnmatch.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>

#include <sstream>

#include <cstring>
using namespace std;


/* Macros */

#define streq(s0, s1) (strcmp((s0), (s1)) == 0)
#define     debug(M) \
    cerr << "[DEBUG] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << ": " << M << endl;
#define log(M) \
    cout << M << endl;
//#define     log(M)
    //cerr << "[LOG] " << __func__ << ": " << M << endl;


/* typedef */
template<typename Key, typename Value>
    using umap = unordered_map<Key, Value>;


/* global variables for open directories and pipefd*/
extern unordered_set<DIR *> openDirs;
extern int pipefd[2];

struct rule {
    string pattern;
    string action;
    
    rule() {}
    rule(string pattern, string action) {
        this->pattern = pattern;
        this->action = action;
    }
};

struct fileInfo {
    time_t mtime; // last modified time
    bool visited;

    fileInfo() {}
    fileInfo(time_t mtime, bool visited) {
        this->mtime = mtime;
        this->visited = visited;
    }
};

int scan(string root, int frequency, umap<string, vector<rule>> & rules);
time_t getMTime(const string filename);
int detect(string filename, umap<string, fileInfo> & fileMap,
           umap<string, vector<rule>> & rules);
int detectDelete(umap<string, fileInfo> & fileMap, umap<string, vector<rule>> & rules);
int executePipe(umap<string, vector<rule>> & rules);
int execute(string filename, string event, umap<string, vector<rule>> & rules);
umap<string, vector<rule>> loadRules(string ruleFile);
void signalHandler(int sig);

#endif
