#include "main.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        testsuite_getopt();
    } else {
        manual_test_getopt(argc, argv);
    }
}
