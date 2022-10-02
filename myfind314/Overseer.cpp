#include "Overseer.hpp"

Overseer::Overseer(int argc, char **argv) {
    if (argc > 2) setFlags(argc, argv);
    else printf("Not enough args!\n");
}

void Overseer::setFlags(int argc, char **argv) {
    int c;
    while((c = getopt(argc, argv, "Rih")) != EOF) {

        switch(c)
        {
            case 'R':
                rec_flag = true;
                printf("switch 'R' specified\n");
                break;

            case 'i':
                case_insensitive_flag = true;
                printf("switch 'i' specified");
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

void Overseer::extractSearchables(int argc, char **argv) {
    vector<string>* input = new vector<string>();
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') input->insert(input->end(), string(argv[i]));
    }
    initiateSearch(&input->at(0), input);
}

void Overseer::initiateSearch(string* path, vector<string>* input) {
    if (input->size() > 2) createChildren(path, input);
}

void Overseer::checkForFile(const string *path, const string *fileName) {
    if (rec_flag) {
        checkFileRec(path, fileName);
    } else {

    }
    string fullPath = *path + *fileName;
    //    printf("Full path: %s\n", fullPath.c_str());
    if (checkFile(&fullPath)) {
        printf("pid: %ld - file %s - path %s\n", (long int)getpid(), fileName->c_str(), fs::current_path().c_str());
    }
}

bool Overseer::checkFile(const string *fileName) {
//    printf("%s\n",fileName->c_str());
    struct stat buffer;
    return (stat (fileName->c_str(), &buffer) == 0);
}

void Overseer::checkFileRec(const string *path, const string *fileName) {
    for (const auto & entry : fs::recursive_directory_iterator(*path)) {
        string fullPath = createFullPath(entry.path().string(), const_cast<string *>(fileName));
        if (checkFile(&fullPath)) {
            printf("pid: %ld - file %s - path %s\n", (long int)getpid(), fileName->c_str(), fullPath.c_str());
        }
    }
}

string Overseer::createFullPath(string path, string* fileName) {
    return path + '/' + *fileName;
}

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
            checkForFile(path, &input->at(i));
            break;
        }
    }
    if (isParent) {
        for (size_t i = 0; i < input->size()-1; ++i) {
            waitpid(pids[i], &status, 0);
        }
    } else {
//        printf("child process\n");
    }
}
