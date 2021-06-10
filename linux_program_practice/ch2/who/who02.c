/*
who01.c - version 0.2 of who
open、read utmp file and show results.
suppresses empty records
formats time nicely
*/
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// #define SHOWHOST                // include remote machine on output

void showInfo(struct utmp *);
void showTime(const long timeval);
int main()
{
    struct utmp cur_record;     // read info into here
    int utmpfd;
    int reclen = sizeof(cur_record);

    if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) // UTMP_FILE is in <utmp.h>, O_RDONLY is in <fcntl.h>
    {
        perror( UTMP_FILE);
        exit(1);
    }
    while (read(utmpfd, &cur_record, reclen) == reclen)
    {
        showInfo(&cur_record);
    }
    close(utmpfd);
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