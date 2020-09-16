/**
 * utmplib.c - functions to buffer reads from utmp file
 *  functions are
 *      utmp_open(filename) - open file(and init some variables)
 *          return -1 on error
 *      utmp_next()                     - return pointer to next struct
 *           return NULL on eof
 *      utmp_close()                    - close file
 */
 
 #include <stdio.h>
 #include <fcntl.h>
 #include <sys/types.h>
 #include <utmp.h>
 #include <unistd.h>
#include "utmplib.h"
 #define    NRRECS  16  // number of records in buffer
 #define    NULLUT  ((struct utmp*) NULL)
 #define    UTSIZE  (sizeof(struct utmp) )

 static char utmpbuf[NRRECS * UTSIZE];          // storage 
 static int num_recs;                                                   // num of records stored
 static int cur_rec;                                                        // next record to go
 static int fd_utmp = -1;                                              // fd read from

 int  utmp_open(char *filename)
 {
     fd_utmp = open(filename, O_RDONLY);    // open it
     cur_rec = num_recs = 0;                                    // init, no recs yet
     return fd_utmp;                                                     
 }

 struct utmp *utmp_next()
 {
     struct utmp *p_rec;
     if (fd_utmp == -1)                                                         // error?
     {
         return NULLUT;     
     }
     
     if (cur_rec == num_recs && utmp_reload() == 0)  // any more?
     {
         return NULLUT;
     }

     p_rec = (struct utmp *) &utmpbuf[cur_rec * UTSIZE]; // get addr of the next record
     cur_rec++;
     return p_rec;
 }

// read next bunch of records into buffer
 int utmp_reload()
 {
     int batch_read;
     batch_read = read(fd_utmp, utmpbuf, NRRECS * UTSIZE);
     num_recs = batch_read/UTSIZE;                  //why not NRRECS?
     cur_rec = 0;
     return num_recs;
 }

 void utmp_close()
 {
     if (fd_utmp != -1)     // don't close if not open
     {
         close(fd_utmp);    
     }
     
 }

