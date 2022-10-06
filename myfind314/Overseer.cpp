#include "Overseer.hpp"

// Constructor with first argument checking
Overseer::Overseer(int argc, char **argv) {
    if (argc > 2) setFlags(argc, argv);
    else printf("Not enough args!\n");
}

// Set flags -i (case insensitive) and -R (search recursively).
// After getting all switches, begin with extracting the search-files from the command
void Overseer::setFlags(int argc, char **argv) {
    int c;
    while((c = getopt(argc, argv, "Rih")) != EOF) {
        switch(c) {
            case 'R':
                rec_flag = true;
                printf("switch 'R' specified\n");
                break;

            case 'i':
//                case_insensitive_flag = true;
                printf("switch 'i' specified\n");
                break;

            case '?':
            case 'h':
            default :
                printf("Usage: ./mf [-R] [-i] searchpath filename1 [filename2] ...[filenameN]\n");
                break;

            case -1:
                break;
        }
    }
    extractSearchables(argc, argv);
}

// Gather all arguments, without an '-' at the beginning => path + fileNames remaining.
// Initiate search, if enough filenames were given (at least 2: path + filename)
void Overseer::extractSearchables(int argc, char **argv) {
    vector<string>* input = new vector<string>();
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') input->insert(input->end(), string(argv[i]));
    }
    if (input->size() < 2) printf("wrong input!\n");
    else initiateSearch(&input->at(0), input);
}

// If more than 1 filename, begin creating children. If not, begin the search alone.
void Overseer::initiateSearch(string* path, vector<string>* input) {
    if (input->size() > 2) createChildren(path, input);
    else defineSearchDepth(path, &input->at(1));
}

// Fork for each filename one child process and wait until all children are done searching.
// Every child will set isParent to false, while only the parent's isParent will remain true.
void Overseer::createChildren(string *path, vector<string> *input) {
    pid_t pids[(int)input->size()-1];
    int status;
    bool isParent = true;
    for (size_t i = 1; i < input->size(); ++i) {
        pids[i-1] = fork();
        if (pids[i-1] < 0) {
            printf("Error forking\n");
        } else if (pids[i-1] == 0) {
            isParent = false;
            defineSearchDepth(path, &input->at(i));
            break;
        }
    }
    // Wait for children to finish their job to prevent a 'zombie apocalypse'
    if (isParent) {
        for (size_t i = 0; i < input->size()-1; ++i) {
            waitpid(pids[i], &status, 0);
        }
    }
}

// if rec_flag => begin recursive search, with all 1. level subdirectories
// then search in this directory
void Overseer::defineSearchDepth(const string *path, const string *fileName) const {
    if (rec_flag) recCheckFile(path, fileName);

    string fullPath = *path + *fileName;
    if (checkFile(&fullPath)) {
        printf("%ld: %s: %s\n", (long int)getpid(), fileName->c_str(),
               createFullPath(fs::current_path(), &fullPath, const_cast<string *>(fileName)).c_str());
    }
}

// stat() = check the status of a file and write it to the buffer.
// fastest way to check for existence of a file: https://stackoverflow.com/a/12774387/14093870
bool Overseer::checkFile(const string *fileName) {
    struct stat buffer{};
    return (stat (fileName->c_str(), &buffer) == 0);
}

// check all files recursively.
void Overseer::recCheckFile(const string *path, const string *fileName) {
    try {
        for (const auto & entry : fs::recursive_directory_iterator(*path)) {
            string fullPath = createSearchPath(entry.path().string(), const_cast<string *>(fileName));
            if (checkFile(&fullPath)) {
                printf("%ld: %s: %s\n", (long int)getpid(), fileName->c_str(),
                       createFullPath(fs::current_path(), &fullPath, const_cast<string *>(fileName)).c_str());
            }
        }
    } catch (...) { }
}

// helper function to create a searchpath
string Overseer::createSearchPath(string path, string* fileName) {
    return path + '/' + *fileName;
}

// helper function to create a path to output
string Overseer::createFullPath(string path, string* fullPath, string* fileName) {
    if (fullPath->at(0) == '.') return path + (fullPath->at(1) == '.' ? fullPath->substr(2) : fullPath->substr(1));
    else return *fullPath;
}
