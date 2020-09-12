/*
more02c - version 0.2 of more
read and print 24 lines then pause for a few special commands
feature of version 0.2: read from /dev/tty for commands
*/
#include <stdio.h>

#define PAGE_LEN 24
#define LINE_LEN 512

void do_more(FILE *);
int see_more(FILE *);
int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    if (argc == 1)
    {
        do_more(stdin);
    }
    else
    {
        while (--argc)
        {           
            if( (fp = fopen(*++argv, "r")) != NULL )
            {
                do_more(fp);
                fclose(fp);
            }
            else
            {
                exit(1);
            }
            
        }
        
    }
    
    return 0;
}

/*
read LINE_LEN lines, and then call see_more() for further instructions
*/
void do_more(FILE *fp)
{
    char line[LINE_LEN];
    int num_of_lines = 0;
    FILE *fp_tty = NULL;
    if( (fp_tty = fopen("/dev/tty", "r")  == NULL))
    {
        exit(1);
    }
    while (fgets(line, LINE_LEN, fp) != NULL)
    {      
        if (num_of_lines == PAGE_LEN)
        {
            int lines = see_more(fp_tty);
            if (lines == 0)
            {
                break;
            }
            num_of_lines -= lines;
        }
        if ((fputs(line, stdout) == EOF))
        {
            exit(1);
        }
        num_of_lines++;
    }
    
}

/*
read a  command from /dev/tty
*/
int see_more(FILE *fcmd)
{
    int ch;
    printf("\003[7m more? \003[m\n");
    while ( (ch = getc(fcmd) ) != EOF)
    {
        if (ch == 'q')
        {
            exit(1);
        }
        if (ch == '\n')
        {
            return 1;
        }
        if (ch == ' ')
        {
            return PAGE_LEN;
        }
    }
    return 0;
}