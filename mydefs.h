/* 
 * File:   mydefs.h
 * Author: Siqi Li
 *
 * Created on January 17, 2015, 3:48 PM
 */

#ifndef MYDEFS_H
#define	MYDEFS_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * flag for dev mode. If dev mode is enabled (1), a lot of logs 
 * starting with "[dev]" will be printed to stdout.
 */
#define DEV_MODE 0
/**
 * flag for whether the program uses signals to kill zombie processes. 
 * If it's set to non-0, then all the processes will be automatically 
 * terminated as soon as they stop. If it's set to 0, then all the 
 * processes will turn into zombie processes when they stop, and will 
 * be killed when the user enters the next command.
 */
#define USE_SIGNAL 1
/** standard max length for this project. */
#define MAX_LEN 1024
/** a string containing all the whitespace characters */
#define WHITESPACE " \t\n\v\f\r"
/** a macro used to calculate the length of an array */
#define ARRLEN(x) (sizeof(x) / sizeof(x[0]))


#ifdef	__cplusplus
}
#endif

#endif	/* MYDEFS_H */

