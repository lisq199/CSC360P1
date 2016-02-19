/* 
 * File:   bgpcounter.c
 * Author: Siqi Li
 *
 * Created on January 24, 2015, 10:00 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "mydefs.h"

static int pcount = 0;
static pid_t pids[MAX_LEN];
static char pnames[MAX_LEN][MAX_LEN];

/**
 * Check if there are no background processes running
 * @return if there are no background processes running
 */
static int bgp_isempty()
{
    int i;
    /* if the pid is 0, then the process is already gone */
    for (i = 0; i < pcount; i++) {
        if (pids[i] > 0) {
            return 0;
        }
    }
    return 1;
}

/**
 * Add a new background process to the record
 * @param pid the new pid
 * @param argc
 * @param argv all the args the user used to start the process
 */
void bgp_add(pid_t pid, int argc, char **argv)
{
    int i;
    /* add the new pid to the end of pids */
    pids[pcount] = pid;
    /* copy every arg in argv to the end of pnames */
    strcpy(pnames[pcount], "");
    for (i = 0; i < argc; i++) {
        strcat(pnames[pcount], argv[i]);
        strcat(pnames[pcount], " ");
    }
    /*
     * mimic the behavior of a real shell by printing the pid of the 
     * new process along with its position in background processes
     */
    printf("[%d] %d\n", ++pcount, pid);
    if (DEV_MODE) {
        printf("[dev] new pid: %d\n", pid);
    }
}

/**
 * Remove a background process from the record by its pid
 * @param pid
 */
void bgp_remove(pid_t pid)
{
    int i;
    int found = 0;
    /* Check if the process to be killed actually exists */
    for (i = 0; i < pcount; i++) {
        if (pids[i] == pid) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return; /* in theory, this will never happen */
    }
    /* At this point, i is the index of the process to be removed */
    printf("[%d] Terminated\t\t%s\n", i + 1, pnames[i]);
    /* set the pid to 0 to indicate the process is gone */
    pids[i] = 0;
    /*
     * If there is no background process after this kill, then 
     * reset everything.
     */
    if (bgp_isempty()) {
        pcount = 0;
    }
}
