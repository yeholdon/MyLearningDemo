/**
 * play_again05.c : ask if user wants another transaction
 * method : set tty into char-by-char mode, no-echo mode 
 * set tty into no-delay mode
 * read char, return result
 * resets terminal modes on SIGINT, ignores SIGQUIT
 * return: 0 =>yes, 1=>no, 2 => timeout
 * better: reset terminal mode on interrupt
*/
#include <stdio.h>
#include <termio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

#define QUESTION "Do you want another transaction"
#define TRIES 3         // max tries
#define SLEEPTIME   2   // time per try
#define BEEP    putchar('\a')   // alert user

int get_response(char *, int);
void set_cr_noecho_mode();
void tty_mode(int how);
int get_ok_char();
void set_nodelay_mode();
void ctrl_c_handler(int);       // handler func of SIGINT

int main()
{
    int response;
    tty_mode(0);    // save tty mode
    set_cr_noecho_mode();  // set ch by ch mode
    set_nodelay_mode();
    signal(SIGINT, ctrl_c_handler);     // handle SIGINT
    signal(SIGQUIT, SIG_IGN);       // ignore QUIT signals
    response = get_response(QUESTION, TRIES);
    tty_mode(1);  // restore tty mode
    return response;
}

/**
 * puepose: ask a question and wait for a y/n answer
 * method: use getchar and ignore non y/n answers or maxtries
 * return: 0=>yes, 1=>no, 2 => timeout
*/
int get_response(char *question, int maxtries)
{
    int input;
    printf("%s (y/n)?", question);
    fflush(stdout);
    while (1)
    {
        sleep(SLEEPTIME);
        input = tolower(get_ok_char());     // get next chr
        if (input == 'y')
        {
            return 0;
        }
        if (input == 'n')
        {
            return 1;
        }
                
        if ( maxtries-- == 0)
        {
            return 2;
        }
        BEEP;
    }
    
}

/**
 * skip over non - legal chars and return y, Y, n, N or EOF
*/
int get_ok_char()
{
    int c;
     while ( ((c = getchar()) != EOF && strchr("yYnN", c) == NULL))
     {
         // no-block or no-delay, getchar() will return immediately. break until get input char  one of "yYnN"
     }
     return c;
}

/**
 * set_crmode() : put file descriptor 0 (i.e. stdin) into char - by - char mode
 * method: use bits in termios
*/
void set_cr_noecho_mode()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag  &= ~ICANON; // no buffer
    ttystate.c_lflag  &= ~ECHO;     // no echo
    ttystate.c_cc[VMIN] = 1;            // get 1 char at a time
    tcsetattr(0, TCSANOW, &ttystate);   // install settings
}

/**
 * put file descriptor 0 into no-delay mode
 * method: use fcntl to set bits 
 * tcsetattr() will do something similar, but it is complicated
*/
void set_nodelay_mode()
{
    int termflags;
    termflags = fcntl(0, F_GETFL);
    termflags |= O_NDELAY;
    fcntl(0, F_SETFL, termflags);
}

void tty_mode(int how)
{
    static  struct termios origin_mode;
    static  int original_flags;
    if (how == 0)
    {
        tcgetattr(0, &origin_mode);
        original_flags = fcntl(0, F_GETFL);
    }
    else
    {
        tcsetattr(0, TCSANOW, &origin_mode);
        fcntl(0, F_SETFL, original_flags);
    }
}

/**
 * called if SIGINT is detected
 * action: reset tty 
*/
void ctrl_c_handler(int signum)
{
    printf("tty is reset!\n");
    tty_mode(1);
    exit(1);
}