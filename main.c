/* 
 * File:   main.c
 * Author: Siqi Li
 *
 * Created on January 17, 2015, 2:32 PM
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "bgpcounter.h"
#include "mycmds.h"
#include "mydefs.h"
#include "myutils.h"

/**
 * A function that catches signals.
 * @param signo Signal number.
 */
static void catch_func(int signo)
{
    if (DEV_MODE) {
        printf("[dev] signal %d caught\n", signo);
    }
    switch (signo) {
        case SIGCHLD:
            kill_zombies();
            /*
             * On linux, the signal needs to be registered again. On OS X, 
             * and possibly other Unix systems, the next line is optional.
             */
            register_signal(SIGCHLD, catch_func);
            break;
        default:
            break;
    }
}

/**
 * Deal with some special commands such as "exit" and "cd". These 
 * commands are run either before fork() or in the child process 
 * depending on whether the commands are to be run in the background.
 * @param argc
 * @param argv The entire array of commands.
 * @param cwd Current working directory.
 * @return 1 if *argv is a special command, or 0 otherwise.
 */
int check_special_cmds(int argc, char** argv, const char *cwd)
{
    if (strcmp(*argv, "exit") == 0) {
        exit(EXIT_SUCCESS);
    } else if (strcmp(*argv, "cd") == 0) {
        mycd(argc, argv, cwd);
    } else {
        /* it's not a special command */
        return 0;
    }
    return 1;
}

/**
 * Run a set of commands using execvp in the child process.
 * @param argc
 * @param argv The entire array of commands.
 * @param cwd Current working directory.
 * @param background Whether it should be a background process.
 */
void run_in_child(int argc, char **argv, const char *cwd, int background)
{
    pid_t pid;
    /* create child process here */
    if ((pid = fork()) < 0) {   /* fork failed */
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  /* child process */
        /*
         * if it's a background process, special commands are run in 
         * the child process in the background.
         */
        if (background && check_special_cmds(argc, argv, cwd)) {
            if (DEV_MODE) {
                puts("[dev] special cmd run in background");
            }
        }
        if (execvp(*argv, argv) < 0) {
            rsi_error(*argv);
            _exit(EXIT_FAILURE);
        }
        /* 
         * Theoretically, the following line is unnecessary. I'm 
         * putting it here just to be safe.
         */
        _exit(EXIT_SUCCESS);
    } else {    /* parent process */
        if (!background) {
            /* 
             * wait for the child process only if the child is not a 
             * background process.
             */
            if (!waitpid(pid, NULL, 0)) {
                perror("waitpid error");
            }
        } else {
            if (DEV_MODE) {
                puts("[dev] & background process");
            }
            bgp_add(pid, argc, argv);
        }
        if (!USE_SIGNAL) {
            /*
             * If USE_SIGNAL is 0, then manually kill zombies every time 
             * the user enters a command.
             */
            kill_zombies();
        }
    }
}

/**
 * Execute the user command.
 * @param input The whole line of user input.
 * @param cwd Current working directory.
 */
void execute(char* input, const char *cwd)
{
    int i = 0;  /* counter */
    int background; /* whether the new process is a background process */
    char *argv[MAX_LEN];    /* an array of strings storing split arguments */
    char *token;    /* temp string for storing tokens */
    /* split the user input by whitespace characters */
    token = strtok(input, WHITESPACE);
    while (token != NULL) {
        argv[i++] = token;
        if (DEV_MODE) {
            printf("[dev] args[%d]: %s\n", i - 1, argv[i - 1]);
        }
        token = strtok(NULL, WHITESPACE);
    }
    /* the string array used for execvp must be NULL terminated */
    argv[i] = NULL;
    /* if the last argument is &, then it's a background process */
    background = strcmp(argv[i - 1], "&") == 0;
    /* After this point, i is argc */
    if (background) {
        /* 
         * if it's a background process, then the "&" is not necessary 
         * for execution.
         */
        argv[--i] = NULL;
    } else {
        /*
         * if it's not a background process, the special commands can be 
         * run in the parent process before fork();
         */
        if (check_special_cmds(i, argv, cwd)) {
            if (DEV_MODE) {
                puts("[dev] special cmd run before fork");
            }
            return;
        }
    }
    /* runInChild() will call fork() */
    run_in_child(i, argv, cwd, background);
}

int main(int argc, char** argv)
{
    char cwd[MAX_LEN];  /* current working directory */
    char input[MAX_LEN];    /* user input */
    if (USE_SIGNAL) {
        /* SIGCHLD is only registered if USE_SIGNAL is non-0 */
        register_signal(SIGCHLD, catch_func);
    }
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        /* if argv[1] is "-h", then start at the home dir */
        gohome();
    }
    while(1) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd error");
            return (EXIT_FAILURE);
        }
        printf("RSI: %s > ", cwd);
        /* flush manually because there wasn't a '\n' in the end */
        fflush(stdout);
        fgets(input, sizeof(input), stdin);
        if (DEV_MODE) {
            printf("[dev] input: %s\n", input);
        }
        if (!is_string_empty(input)) {
            execute(input, cwd);
        }
    }
    return (EXIT_SUCCESS);
}
