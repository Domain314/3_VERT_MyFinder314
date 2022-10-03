#include "myfind314/Overseer.hpp"

// Keep it short and simple.
int main(int argc, char *argv[]) {
    Overseer* overseer = new Overseer(argc, argv);
    delete(overseer);
    return 0;
}