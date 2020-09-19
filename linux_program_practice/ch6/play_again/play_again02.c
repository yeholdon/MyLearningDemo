/**
 * play_again02.c : ask if user wants another transaction
 * method : set tty into char-by-char mode, read char, return result
 * return: 0 =>yes, 1=>no
 * better: do no echo inappropriate
*/
#include <stdio.h>
#include <termio.h>

#define QUESTION "Do you want another transaction"
int get_response(char *);
void set_crmode();
void tty_mode(int how);

int main()
{
    int response;
    tty_mode(0);    // save tty mode
    set_crmode();  // set ch by ch mode
    response = get_response(QUESTION);
    tty_mode(1);  // restore tty mode
    return response;
}

/**
 * puepose: ask a question and wait for a y/n answer
 * method: use getchar and ignore non y/n answers
 * return: 0=>yes, 1=>no
*/
int get_response(char *question)
{
    int input;
    printf("%s (y/n)?", question);
    while (1)
    {
        switch(input = getchar())
        {
            case 'y':
            case 'Y':   return 0;
            case 'n':   
            case 'N':  
            case EOF:   return 1;
            default:
            printf("\ncannot understand %c, ", input);
            printf("Please type y or no\n");
        }
    }
    
}

/**
 * set_crmode() : put file descriptor 0 (i.e. stdin) into char - by - char mode
 * method: use bits in termios
*/
void set_crmode()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag  &= ~ICANON; // no buffer
    ttystate.c_cc[VMIN] = 1;            // get 1 char at a time
    tcsetattr(0, TCSANOW, &ttystate);   // install settings
}

void tty_mode(int how)
{
    static  struct termios origin_mode;
    if (how == 0)
    {
        tcgetattr(0, &origin_mode);
    }
    else
    {
        tcsetattr(0, TCSANOW, &origin_mode);
    }
}