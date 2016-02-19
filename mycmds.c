/* 
 * File:   mycmds.c
 * Author: Siqi Li
 *
 * Created on January 18, 2015, 10:12 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mydefs.h"
#include "myutils.h"

/**
 * Go to the home directory.
 */
void gohome()
{
    char errmsg[MAX_LEN];
    char homedir[MAX_LEN];
    get_homedir(homedir, sizeof(homedir));
    if (chdir(homedir)) {
        strcpy(errmsg, "cd: ");
        strcat(errmsg, homedir);
        rsi_error(errmsg);
    }
}

/**
 * My implementation of cd.
 * @param argc
 * @param argv The entire array of arguments (including "cd").
 * @param cwd Current working directory.
 */
void mycd(int argc, char **argv, const char *cwd)
{
    char newdir[MAX_LEN];
    char errmsg[MAX_LEN];
    char homedir[MAX_LEN];
    if (argc > 1) {
        /*
         * if the user passed any args to cd, then anything except 
         * the first arg (not counting "cd") is ignored.
         */
        if (argv[1][0] == '/') {
            /*
             * if the arg starts with '/', then the new directory 
             * should start at root.
             */
            strcpy(newdir, argv[1]);
        } else if (strcmp(argv[1], "~") == 0) {
            /* if the arg is "~", then go home */
            gohome();
            return;
        } else if (argv[1][0] == '~' && argv[1][1] == '/') {
            /*
             * if the arg starts with "~/", then the new directory 
             * should start at home.
             */
            get_homedir(homedir, sizeof(homedir));
            strcpy(newdir, homedir);
            strcat(newdir, argv[1] + 1);
        } else {
            /* else, the new directory should start at cwd */
            strcpy(newdir, cwd);
            strcat(newdir, "/");
            strcat(newdir, argv[1]);
        }
        if (DEV_MODE) {
            printf("[dev] newdir: %s\n", newdir);
        }
        if (chdir(newdir)) {    /* chdir returns 0 on success */
            strcpy(errmsg, argv[0]);
            strcat(errmsg, ": ");
            strcat(errmsg, argv[1]);
            rsi_error(errmsg);
        }
    } else {    /* if the user is calling cd without args */
        gohome();
    }
}
