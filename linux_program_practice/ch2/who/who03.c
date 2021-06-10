/*
who03.c - version 0.3 of who
open、read utmp file and show results.
suppresses empty records
formats time nicely
*buffer input (use funcs in utmplib)
*/
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "utmplib.h"
// #define SHOWHOST                // include remote machine on output

void showInfo(struct utmp *);
void showTime(const long timeval);
int main()
{
	struct utmp	*utbufp,	// holds pointer to next rec
			*utmp_next();	// returns pointer to next

	if ( utmp_open( UTMP_FILE ) == -1 ){
		perror(UTMP_FILE);
		exit(1);
	}
	while ( ( utbufp = utmp_next() ) != ((struct utmp *) NULL) )
		showInfo( utbufp );
	utmp_close( );
	return 0;
}
/* shwoInfo()
 *      displays the contents od the utmp struct
 *      in human readable format
 *      * displays nothing if the ut_type is not USER_PROCESS
 */
void showInfo(struct utmp *u)
{
    if (u->ut_type != USER_PROCESS)
    {
        return;
    }
    
    printf("%-8.8s", u->ut_user);
    printf(" ");
    printf("%-8.8s", u->ut_line);
    printf(" ");
    // printf("%10d", u->ut_tv.tv_sec);
    // printf(" ");
    showTime(u->ut_tv.tv_sec);
#ifdef SHOWHOST
    printf("(%s)", u->ut_host);
#endif
    printf("\n");        
}

/**
 * displays time in a format fit for human consumption
 * uses ctime to build a string then picks part of it
 * to 12 chars
*/
void showTime(const long  timeval)
{
    char *pTime = ctime(&timeval);  // to hold address of  time
    // convert time to string
    // string looks like
    // Mon Feb
    printf ("%12.12s", pTime + 4);
}