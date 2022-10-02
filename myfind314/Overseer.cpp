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
                rec_flag = 1;
                printf("switch 'R' specified\n");
                break;

            case 'i':
                case_sensitive_flag = 0;
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

    for (u_long i = 1; i < input->size(); ++i) {
        printf("entry: %s\n", input->at(i).c_str());
        checkForFile(path, &input->at(i));

    }
//    fs::path searchPath = fs::path(path + "/tester");
}

void Overseer::checkForFile(const string *path, const string *fileName) {
    string fullPath = *path + *fileName;
    printf("Full path: %s\n", fullPath.c_str());
    if (checkFile(&fullPath)) {
        printf("pid: %ld - file %s - path %s\n", (long int)getpid(), fileName->c_str(), fs::current_path().c_str());
    }

//    if(fs::status_known(s) ? fs::exists(s) : fs::exists(p)) {
//        printf("exists\n");
//    }
//    else {
//        printf("not existing\n");
//    }
}

bool Overseer::checkFile(const string *fileName) {
//    printf("%s\n",fileName->c_str());
    struct stat buffer;
    return (stat (fileName->c_str(), &buffer) == 0);
}

void Overseer::createChildren(string *path, vector<string> *input) {
    pid_t pid = 0;
    
}









//    string currentPath = fs::current_path().string();
//    fs::path searchPath = fs::path(currentPath + "/tester");
//    printf("current path: %s\n", currentPath.c_str());
//    for (const auto & entry : fs::recursive_directory_iterator(currentPath)) {
//        checkForFile(entry, entry.status());
//    }
//        printf("entry path: %s\n", entry.path().c_str());



//    pid_t childpid;
//    childpid = fork();
//    if (childpid == 0) {
//        printf("I'm a child %ld\n", (long)getpid());
//    } else {
//        printf("I'm a parent %ld\n", (long)getpid());
//    }


//    for (int i = 0; i < argc; ++i) {
//        printf("arg: %s\n", argv[i]);
//    }
// note the colon (:) to indicate that 'b' has a parameter and is not a switch