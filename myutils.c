/* 
 * File:   myutils.c
 * Author: Siqi Li
 *
 * Created on January 18, 2015, 10:31 AM
 */

#include <ctype.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "bgpcounter.h"
#include "mydefs.h"

/**
 * Check if a string is "" or only contains whitespace characters.
 * @param s
 * @return 1 if the string is empty, or 0 otherwise.
 */
int is_string_empty(const char *s)
{
    while (*s != '\0') {
        if (!isspace(*s++)) {
            return 0;
        }
    }
    return 1;
}

/**
 * Get the home directory of the current user
 * @return the home directory of the current user
 */
char* get_homedir(char* homedir, int len)
{
    struct passwd *pw = getpwuid(getuid());
    char *hdir = pw->pw_dir;
    if (DEV_MODE) {
        printf("[dev] homedir: %s\n", hdir);
    }
    strncpy(homedir, hdir, len);
    if (DEV_MODE) {
        printf("[dev] copied homedir: %s\n", homedir);
    }
    return hdir;
}

/**
 * Kill all the zombie processes.
 */
void kill_zombies()
{
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        /*printf("PID %d ", pid);
        if (WIFEXITED(status)) {
            puts("exited");
        } else if (WIFSIGNALED(status)) {
            puts("signaled");
        } else if (WIFSTOPPED(status)) {
            puts("stopped");
        } else if (WIFCONTINUED(status)) {
            puts("continued");
        }*/
        bgp_remove(pid);
    }
}

/**
 * An easier way of calling perror with an error message.
 * @param arg The error message will be -RSI: <arg>: <...>
 */
void rsi_error(const char* arg)
{
    char errmsg[MAX_LEN];
    strcpy(errmsg, "-RSI: ");
    strcat(errmsg, arg);
    perror(errmsg);
}

/**
 * An easier way of registering a signal.
 * @param signo the signal number to be registered
 * @param f a pointer to the function to be called
 */
void register_signal(int signo, void (*f)(int))
{
    if (signal(signo, f) == SIG_ERR) {
        perror("signal error");
        exit(EXIT_FAILURE);
    }
}
