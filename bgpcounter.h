/* 
 * File:   bgpcounter.h
 * Author: Siqi Li
 *
 * Created on January 24, 2015, 10:26 AM
 */

#ifndef BGPCOUNTER_H
#define	BGPCOUNTER_H

#ifdef	__cplusplus
extern "C" {
#endif

    void bgp_add(pid_t pid, int argc, char **argv);
    void bgp_remove(pid_t pid);


#ifdef	__cplusplus
}
#endif

#endif	/* BGPCOUNTER_H */

