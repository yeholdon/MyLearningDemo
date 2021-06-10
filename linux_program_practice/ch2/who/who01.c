/*
who01.c - version 0.1 of who
open、read utmp file and show results.
*/
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define SHOWHOST                // include remote machine on output

void showInfo(struct utmp *);
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

void showInfo(struct utmp *u)
{
    printf("%-8.8s", u->ut_user);
    printf(" ");
    printf("%-8.8s", u->ut_line);
    printf(" ");
    printf("%10d", u->ut_tv.tv_sec);
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", u->ut_host);
#endif
    printf("\n");        
}