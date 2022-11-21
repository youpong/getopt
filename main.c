#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *OPT = getenv("OPT"); // "LONG" | "SHORT"

    void (*manual_test)(int, char *const *) = manual_test_getopt;
    if (OPT && !strcmp(OPT, "LONG")) {
        manual_test = manual_test_getopt_long;
    }

    if (argc != 1) {
        manual_test(argc, argv);
        return 0;
    }

    if (testsuite_getopt() || testsuite_getopt_long()) {
        printf("Failed to pass all tests.\n");
        return 1;
    }

    printf("All tests passed!\n");
    return 0;
}
