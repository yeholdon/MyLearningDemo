/**
 * echostate.c : 
 * report current state of echo bit in 
 * tty driver for fd . show how to read attribute 
 * from driver and test a bit
*/
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
int main()
{
    struct termios info;
    if ( tcgetattr(0, &info) == -1)
    {
        perror("tcgetattr");
        exit(1);
    }
    if (info.c_lflag & ECHO)
    {
        printf("echo is on, since its bit is 1\n");
    }
    else
    {
        printf("echo is OFF, since its bit is 0\n");
    }
    
}