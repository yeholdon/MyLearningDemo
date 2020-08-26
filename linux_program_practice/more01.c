/*
more01.c - version 0.1 of more
read and print 24 lines then pause for a few special commands
*/

#include <stdio.h>
#define PAGE_LEN 24
#define LINE_LEN 512
void do_more(FILE*);
int see_more();
int main(int argc, char* argv[])
{
	FILE* fp = NULL;
	if (argc == 1)
	{
		do_more(stdin);
	}
	else
	{
		while (--argc)
		{
			if ( (fp = fopen(*(++argv), "r")) != NULL)
			{
				do_more(fp);
				fclose(fp);
			}
			else
			{
				exit(1); // process termination, exit(int status) <==> main() return status 
			}
		}
	}
	return 0;
}

/*
read PAGE_LEN lines, then call see_more() for further instructions
*/
void do_more(FILE* fp)
{
	char line[LINE_LEN];
	int num_of_lines = 0;
	int see_more(), reply;
	while ( fgets(line, LINE_LEN, fp) ) // more input, read line by line
	{
		if (num_of_lines == PAGE_LEN)	// full screen?
		{
			reply = see_more();			// y: ask user; reply for user instruction
			if ( reply == 0 )
			{
				break;					// n: done, for quit
			}
			num_of_lines -= reply;		// reset count
		}
		if ( fputs( line, stdout ) == EOF ) // show line
		{
			exit(1);						// or die
		}
		num_of_lines++;						// normal state: handle next line
	}
}

/*
print message, wait for response, return # of lines to advance
q means quit, space means next page, Enter means one line 
*/
int see_more()
{
	int c;
	printf("\033[7m more?\033[m");			// reverse display on vt100 terminal
	while ( (c = getchar() ) != EOF )
	{
		if ( c == 'q' )
		{
			return 0;		// q for quit-return
		}
		if ( c == ' ' )
		{
			return PAGE_LEN;	// space for next page
		}
		if ( c == '\n' )		// Enter for 1 line
		{
			return 1;
		}
	}
}