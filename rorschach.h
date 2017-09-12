/* rorschach.h */

#ifndef RORSCHACH_H
#define RORSCHACH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>

#include <cstring>
using namespace std;


/* Macros */

#define streq(s0, s1) (strcmp((s0), (s1)) == 0)
#define     debug(M) \
    cerr << "[DEBUG] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << ": " << M << endl;
#define     log(M) \
    cerr << "[LOG] " << __func__ << ": " << M << endl;


/* typedef */
template<typename Key, typename Value>
    using umap = unordered_map<Key, Value>;


struct rule {
    string pattern;
    string action;
    
    rule() {}
    rule(string _pattern, string _action) {
        this->pattern = _pattern;
        this->action = _action;
    }
};

struct fileInfo {
    time_t mtime; // last modified time
    bool visited;

    fileInfo() {}
    fileInfo(time_t _mtime, bool _visited) {
        this->mtime = _mtime;
        this->visited = _visited;
    }
};

int scan(const string root, int frequency, const umap<string, vector<rule>> & rules);
time_t getMTime(const string filename);
int detect(const string filename, umap<string, fileInfo> & fileMap,
           const umap<string, vector<rule>> & rules);
int detectDelete(umap<string, fileInfo> & fileMap, const umap<string, vector<rule>> & rules);
int execute(const string filename, const string event, const umap<string, vector<rule>> & rules);
umap<string, vector<rule>> loadRules(string ruleFile);
void signalHandler(int sig);

#endif
