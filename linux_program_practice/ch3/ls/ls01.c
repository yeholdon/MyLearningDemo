/** ls01.c - version 0.1 of ls
 *  list contents of directory or directories
 *  if no args, use . as the arg, else list files in args
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char *);

int main(int ac, char *av[])
{
    if (ac == 1)
    {
        do_ls(".");
    }
    else
    {
        while (--ac)
        {
            printf("%s:\n", *++av);
            do_ls(*av);
        }
        
    }
    return 0;
}

void do_ls(char *dirname)
{
    DIR *p_dir;
    struct dirent   *p_dirent;
    if ((p_dir = opendir(dirname) )  == NULL)
    {
        fprintf(stderr, "ls01: cannot open %s\n", dirname);
    }
    else
    {
        while ((p_dirent = readdir(p_dir)) != NULL)
        {
            printf("%s\n", p_dirent->d_name);
        }
        closedir(p_dir);
    }
    
       
}