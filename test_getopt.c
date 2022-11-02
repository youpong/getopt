#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

/* int getopt(int argc, char * const argv[], const char *optstring); */
void testsuite_getopt() {}

void manual_test_getopt(int argc, char* const argv[]) {
    int opt;
    /* char *arg; */
    
    printf("\nGETOPT\n");

    while ((opt = getopt(argc, argv, "abc:d::")) != -1) {
        if (opt == '?') {
            printf("%s: option error\n", argv[0]);
        }
        printf("%c (%d) = '%s'\n", opt, optind, optarg);
    }
    printf("optind = %d\n", optind);
    /* while ((arg = optparse_arg(&options))) { */
    while (argv[optind]) {
        printf("argument: %s\n", argv[optind++]);
    }
}
