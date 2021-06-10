/**
 *  utmplib.h - declaration of functions to buffer reads from utmp file
 *  functions are
 *      utmp_open(filename) - open file(and init some variables)
 *          return -1 on error
 *      utmp_next()                     - return pointer to next struct
 *           return NULL on eof
 *      utmp_close()                    - close file
 */
 

 int  utmp_open(char *filename);

struct utmp *utmp_next();

int utmp_reload();

void utmp_close();