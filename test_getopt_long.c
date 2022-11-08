#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct config {
    char amend;  // OPTARG_NONE
    char brief;  // OPTARG_NONE
    char *color; // OPTARG_OPTIONAL
    int delay;   // OPTARG_REQUIRED
    int erase;   // OPTARG_NONE
};

/**
 * return err
 */
char *parse_long_option(int argc, char *argv[], struct config *conf) {
    int opt;
    char *err = 0;

    optind = 1;
    static struct option longopts[] = {
        {"amend", no_argument, NULL, 'a'},
        {"brief", no_argument, NULL, 'b'},
        {"color", optional_argument, NULL, 'c'},
        {"delay", required_argument, NULL, 'd'},
        {"erase", no_argument, NULL, 0},
        {0,0,0,0},
    };
    int option_index;

    while ((opt = getopt_long(argc, argv, ":abc::d:", longopts,
                              &option_index)) != -1) {
        switch (opt) {
        case 'a':
            conf->amend = 1;
            break;
        case 'b':
            conf->brief = 1;
            break;
        case 'c':
            conf->color = optarg ? optarg : "";
            break;
        case 'd':
            conf->delay = atoi(optarg);
            break;
        case 'e':
            conf->erase++;
            break;
        case '?': // unknown opt
            err = malloc(strlen(argv[0]) + 256);
            sprintf(err, "%s: invalid option -- '%c'\n", argv[0], optopt);
            break;
        case ':': // missing optarg
            err = malloc(strlen(argv[0]) + 256);
            sprintf(err, "%s: missing optarg -- '%c'\n", argv[0], optopt);
            break;
        }
    }

    return err;
}
