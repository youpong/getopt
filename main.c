#include "main.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 1) {
        manual_test_getopt(argc, argv);
        return 0;
    }

    if (testsuite_getopt() == 0) {
        printf("All test passed\n");
    }
}
