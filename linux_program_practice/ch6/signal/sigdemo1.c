/**
 * sigdemo1.c - shows how a signal handler works.
 * - run this and press Ctrl - C a few times
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    void f(int);
    int i;
    signal(SIGINT, f);
    for (size_t i = 0; i < 5; i++)
    {
        printf("hello\n");
        sleep(1);
    }
    
}

void f(int signum)
{
    printf("SIGINT is generated!! \n");
}