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

char *parse_long_option(int argc, char *const argv[], struct config *conf);

static void print_config(struct config *conf) {
    printf("config\n"
           "    amend: %d\n"
           "    brief: %d\n"
           "    color: \"%s\"\n"
           "    delay: %d\n"
           "    erase: %d\n",
           conf->amend, conf->brief, conf->color ? conf->color : "(null)",
           conf->delay, conf->erase);
}

static int get_argc(char *argv[]) {
    char **p = argv;
    while (*p != NULL)
        p++;

    return p - argv;
}

void manual_test_getopt_long(int argc, char *const argv[]) {

    printf("getopt_long\n");
    for (char *const *p = argv; *p != NULL; p++) {
        printf("%s ", *p);
    }
    printf("\n");

    struct config conf = {0};

    char *err = parse_long_option(argc, argv, &conf);
    if (err) {
        printf("%s\n", err);
    }

    print_config(&conf);

    printf("optind = %d\n", optind);
    for (int i = 0; argv[optind + i]; i++) {
        printf("argument[%d]: %s\n", i, argv[optind + i]);
    }
}

/**
 * return err
 */
char *parse_long_option(int argc, char *const argv[], struct config *conf) {
    int opt;
    char *err = 0;

    optind = 1;
    static struct option longopts[] = {
        {"amend", no_argument, NULL, 'a'},
        {"brief", no_argument, NULL, 'b'},
        {"color", optional_argument, NULL, 'c'},
        {"delay", required_argument, NULL, 'd'},
        {"erase", no_argument, NULL, 'e'},
        {0, 0, 0, 0},
    };
    int option_index;

    while ((opt = getopt_long(argc, argv, ":abc::d:e", longopts,
                              &option_index)) != -1) {
        switch (opt) {
        case 0:
            printf("option %s", longopts[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;

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
            if (optopt == 0) { // long option
                sprintf(err, "%s: invalid option -- '%s'\n", argv[0],
                        argv[optind - 1] + 2);
            } else {
                sprintf(err, "%s: invalid option -- '%c'\n", argv[0], optopt);
            }
            break;
        case ':': // missing optarg
            err = malloc(strlen(argv[0]) + 256);
            sprintf(err, "%s: missing optarg -- '%c'\n", argv[0], optopt);
            break;
        }
    }

    return err;
}

int testsuite_getopt_long() {
    struct {
        char *name;
        char *argv[8];
        struct config conf;
        char *args[8];
        char *err;
    } t[] = {
        // clang-format off
        { "0",
            {"", "--", "foobar", 0},
            {0, 0, 0, 0, 0},
            {"foobar", 0},
            NULL,
        },
        { "1",
            {"", "-a", "-b", "-c", "-d", "10", "-e", 0},
            {1, 1, "", 10, 1},
            {0},
            NULL,
        },
        { "2",
            {"", "--amend", "--brief", "--color", "--delay", "10", "--erase", 0},
            {1, 1, "", 10, 1},
            {0},
            NULL,
        },
        { "3",
            {"", "-a", "-b", "-cred", "-d", "10", "-e", 0},
            {1, 1, "red", 10, 1},
            {0},
            NULL,
        },
        { "4",
            {"", "-abcblue", "-d10", "foobar", 0},
            {1, 1, "blue", 10, 0},
            {"foobar", 0},
            NULL,
        },
        { "5",
            {"", "--color=red", "-d", "10", "--", "foobar", 0},
            {0, 0, "red", 10, 0},
            {"foobar", 0},
            NULL,
        },
        { "6",
            {"", "-eeeeee", 0},
            {0, 0, 0, 0, 6},
            {0},
            NULL,
        },
        /* { "7",
            {"", "--delay", 0},
            {0, 0, 0, 0, 0},
            {0},
            ": missing optarg -- \"delay\"\n"
            },*/
        { "70",
            {"", "-d", "-e", 0},
            {0, 0, 0, 0, 0},
            {0},
            0
        },/* done */
        { "8",
            {"", "--foo", "bar", 0},
            {0, 0, 0, 0, 0},
            {"--foo", "bar", 0},
            ": invalid option -- 'foo'\n"
        },
        { "80",
            {"", "-f", "foo", 0},
            {0, 0, 0, 0, 0},
            {"foo", 0},
            ": invalid option -- 'f'\n"
        },
        { "9",
            {"", "-x", 0},
            {0, 0, 0, 0, 0},
            {"-x", 0},
            ": invalid option -- 'x'\n"
        },
        { "10",
            {"", "-", 0},
            {0, 0, 0, 0, 0},
            {"-", 0},
            NULL
        },
        { "11",
            {"", "-e", "foo", "bar", "baz", "-a", "quux", 0},
            {1, 0, 0, 0, 1},
            {"foo", "bar", "baz", "quux", 0},
            NULL
        },
        { "12",
            {"", "foo", "--delay", "1234", "bar", "-cred", 0},
            {0, 0, "red", 1234, 0},
            {"foo", "bar", 0},
            NULL,
        },
    }; // clang-format on
    int ntests = sizeof(t) / sizeof(*t);
    int i, nfails = 0;

    for (i = 0; i < ntests; i++) {
        struct config conf = {0};

        char **argv = t[i].argv;
        int argc = get_argc(t[i].argv);
        char *err = parse_long_option(argc, argv, &conf);

        if (conf.amend != t[i].conf.amend) {
            nfails++;
            printf("FAIL (%s): expected amend %d, got %d\n", t[i].name,
                   t[i].conf.amend, conf.amend);
        }

        if (conf.brief != t[i].conf.brief) {
            nfails++;
            printf("FAIL (%s): expected brief %d, got %d\n", t[i].name,
                   t[i].conf.brief, conf.brief);
        }

        if (t[i].conf.color) {
            if (!conf.color || strcmp(conf.color, t[i].conf.color)) {
                nfails++;
                printf("+FAIL (%s): expected color %s, got %s\n", t[i].name,
                       t[i].conf.color, conf.color ? conf.color : "(nil)");
            }
        } else {
            if (conf.color) {
                nfails++;
                printf("FAIL (%s): expected no color, got %s\n", t[i].name,
                       conf.color);
            }
        }

        if (conf.delay != t[i].conf.delay) {
            nfails++;
            printf("FAIL (%s): expected delay %d, got %d\n", t[i].name,
                   t[i].conf.delay, conf.delay);
        }

        if (conf.erase != t[i].conf.erase) {
            nfails++;
            printf("FAIL (%s): expected erase %d, got %d\n", t[i].name,
                   t[i].conf.erase, conf.erase);
        }

        if (t[i].err) {
            if (!err || strncmp(err, t[i].err, strlen(t[i].err))) {
                nfails++;
                printf("FAIL (%s): expected error \"%s\", got \"%s\"\n",
                       t[i].name, t[i].err, err && err[0] ? err : "(nil)");
            }

        } else {
            if (err) {
                nfails++;
                printf("FAIL (%s): expected no error, got %s\n", t[i].name,
                       err);
            }

            char *arg;
            for (int j = 0; t[i].args[j]; j++) {
                arg = argv[optind++];
                if (!arg || strcmp(arg, t[i].args[j])) {
                    nfails++;
                    printf("FAIL (%s): expected arg %s, got %s\n", t[i].name,
                           t[i].args[j], arg ? arg : "(nil)");
                }
            }
            if ((arg = argv[optind])) {
                nfails++;
                printf("FAIL (%s): expected no more args, got %s\n", t[i].name,
                       arg);
            }
        }
    }

    return nfails;
}
