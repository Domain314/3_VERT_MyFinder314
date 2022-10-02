#ifndef MYFIND314_OVERSEER_HPP
#define MYFIND314_OVERSEER_HPP

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

using namespace std;

class Overseer {

private:
    bool rec_flag = false;
//    bool case_insensitive_flag = false;

    void setFlags(int argc, char **argv);
    void extractSearchables(int argc, char **argv);

    void createChildren(string* path, vector<string>* input);

    void initiateSearch(string* path, vector<string>* input);
    void defineSearchDepth(const string* path, const string *fileName);

    bool checkFile(const string *fileName);
    void recCheckFile(const string *path, const string *fileName);

    string createFullPath(string path, string* fileName);

public:
    Overseer(int argc, char *argv[]);




};


#endif //MYFIND314_OVERSEER_HPP
