/* rorschach.h */

#ifndef RORSCHACH_H
#define RORSCHACH_H

#include <string>
#include <unordered_map>
using namespace std;


/* Macros */

#define streq(s0, s1) (strcmp((s0), (s1)) == 0)
#define     debug(M, ...) \
    fprintf(stderr, "DEBUG %s:%d:%s: " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define     log(M, ...) \
    fprintf(stderr, "LOG %s " M "\n", __func__, ##__VA_ARGS__)


/* typedef */
template<typename Key, typename Value>
    using umap = unordered_map<Key, Value>;


struct rule {
    string pattern;
    string action;
};

struct fileInfo {
    time_t mtime; // last modified time
    bool visited;
};

int scan(const string root, int frequency, const umap<string, rule> & rules);
int detect(const string filename, umap<string, fileinfo> & fileMap,
           const umap<string, rule> & rules);
int detectDelete(umap<string, fileinfo> & fileMap);
int execute(const string filename, const umap<string, rule> & rules);
umap<string, rule> parseRules(const string ruleFile);
void signalHandler(int sig);

#endif
