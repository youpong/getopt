//#include <bits/getopt_ext.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

void testsuite_getopt() {}

void manual_test_getopt(int argc, char* const argv[]) {
    int opt;
    opterr = 0; // suppress error message
    printf("\nGETOPT\n");

    while ((opt = getopt(argc, argv, "abc:d::")) != -1) {
        if (opt == '?') {
            printf("%s: invalid option -- '%c'\n", argv[0], optopt);
        }
        printf("%c (%d) = '%s'\n", opt, optind, optarg);
    }
    printf("optind = %d\n", optind);
    while (argv[optind]) {
        printf("argument: %s\n", argv[optind++]);
    }
}
