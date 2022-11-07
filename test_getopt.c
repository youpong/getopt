#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void manual_test_getopt(int argc, char *const argv[]) {
    int opt;
    opterr = 0; // 0: suppress error message
    printf("getopt\n");

    while ((opt = getopt(argc, argv, ":abc:d::")) != -1) {
        switch (opt) {
        case '?':
            printf("%s: invalid option -- '%c'\n", argv[0], optopt);
            continue;
        case ':':
            printf("%s: missing optarg -- '%c'\n", argv[0], optopt);
            continue;
        default:
            printf("%c (%d) = '%s'\n", opt, optind, optarg);
        }
    }
    printf("optind = %d\n", optind);
    while (argv[optind]) {
        printf("argument: %s\n", argv[optind++]);
    }
}

static int get_argc(char *argv[]) {
    char **p = argv;
    while (*p != NULL)
        p++;

    return p - argv;
}

int testsuite_getopt() {
    struct config {
        char amend;  // OPTARG_NONE
        char brief;  // OPTARG_NONE
        char *color; // OPTARG_OPTIONAL
        int delay;   // OPTARG_REQUIRED
        int erase;   // OPTARG_NONE
    };
    struct {
        char *name;
        char *argv[8];
        struct config conf;
        char *args[8];
        char *err;
    } t[] = {
        // clang-format 0ff
        {"0", {"", "--", "foobar", 0}, {0, 0, 0, 0, 0}, {"foobar", 0}, 0},
        {"1",
         {"", "-a", "-b", "-c", "-d", "10", "-e", 0},
         {1, 1, "", 10, 1},
         {0},
         0},
        /* {
            {"", "--amend", "--brief", "--color", "--delay", "10", "--erase",
           0}, {1, 1, "", 10, 1}, {0}, 0},*/
        {"3",
         {"", "-a", "-b", "-cred", "-d", "10", "-e", 0},
         {1, 1, "red", 10, 1},
         {0},
         0},
        {"4",
         {"", "-abcblue", "-d10", "foobar", 0},
         {1, 1, "blue", 10, 0},
         {"foobar", 0},
         0},
        /* { "5", {"", "--color=red", "-d", "10", "--", "foobar", 0},
         {0, 0, "red", 10, 0},
         {"foobar", 0},
         0}, */
        {"6", {"", "-eeeeee", 0}, {0, 0, 0, 0, 6}, {0}, 0},
        /*
        { "7",
          {"", "--delay", 0},
          {0, 0, 0, 0, 0},
          {0},
          OPTPARSE_MSG_MISSING
        },
        { "8",
          {"", "--foo", "bar", 0},
          {0, 0, 0, 0, 0},
          {"--foo", "bar", 0},
          OPTPARSE_MSG_INVALID
        },
        { "9",
          {"", "-x", 0},
          {0, 0, 0, 0, 0},
          {"-x", 0},
          OPTPARSE_MSG_INVALID
        },*/
        {"10", {"", "-", 0}, {0, 0, 0, 0, 0}, {"-", 0}, 0},
        {"11",
         {"", "-e", "foo", "bar", "baz", "-a", "quux", 0},
         {1, 0, 0, 0, 1},
         {"foo", "bar", "baz", "quux", 0},
         0},
        /* { "12",
          {"", "foo", "--delay", "1234", "bar", "-cred", 0},
          {0, 0, "red", 1234, 0},
          {"foo", "bar", 0},
          0
        },*/
    }; // clang-format 0n
    int ntests = sizeof(t) / sizeof(*t);
    int i, nfails = 0;

    for (i = 0; i < ntests; i++) {
        int j, opt;
        char *arg, *err = 0;
        struct config conf = {0, 0, 0, 0, 0};

        int argc = get_argc(t[i].argv);
        char **argv = t[i].argv;

        optind = 1;
        while ((opt = getopt(argc, t[i].argv, ":abc::d:e")) != -1) {
            switch (opt) {
            case 'a':
                conf.amend = 1;
                break;
            case 'b':
                conf.brief = 1;
                break;
            case 'c':
                conf.color = optarg ? optarg : "";
                break;
            case 'd':
                conf.delay = atoi(optarg);
                break;
            case 'e':
                conf.erase++;
                break;
            case '?': // unknown opt
                // TODO
                break;
            case ':': // missing optarg
                // TODO
                break;
                //                err = options.errmsg;
            }
        }

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
                printf("FAIL (%s): expected error '%s', got %s\n", t[i].name,
                       t[i].err, err && err[0] ? err : "(nil)");
            }

        } else {
            if (err) {
                nfails++;
                printf("FAIL (%s): expected no error, got %s\n", t[i].name,
                       err);
            }

            for (j = 0; t[i].args[j]; j++) {
                arg = argv[optind++];
                if (!arg || strcmp(arg, t[i].args[j])) {
                    nfails++;
                    printf("FAIL (%s): expected arg %s, got %s\n", t[i].name,
                           t[i].args[j], arg ? arg : "(nil)");
                }
            }
            /*
            if ((arg = optparse_arg(&options))) {
                nfails++;
                printf("FAIL (%s): expected no more args, got %s\n",
                       t[i].name, arg);
            }*/
        }
    }

    return nfails;
}
