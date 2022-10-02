#include "myfind314/Overseer.hpp"


int main(int argc, char *argv[]) {

//    for (int i = 0; i < argc; ++i) {
//        printf("%s\n", argv[i]);
//    }

    Overseer* overseer = new Overseer(argc, argv);
    delete(overseer);
    return 0;
}