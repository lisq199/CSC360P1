/* 
 * File:   myutils.h
 * Author: Siqi Li
 *
 * Created on January 18, 2015, 10:31 AM
 */

#ifndef MYUTILS_H
#define	MYUTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

    int is_string_empty(const char *s);
    char* get_homedir(char* homedir, int len);
    void kill_zombies();
    void rsi_error(const char* arg);
    void register_signal(int signo, void (*f)(int));


#ifdef	__cplusplus
}
#endif

#endif	/* MYUTILS_H */

