/** ls02.c - version 0.2 of ls
 *  list contents of directory or directories
 *  if no args, use . as the arg, else list files in args
 * support "ls -l"
*/

#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>

void dostat(char *);
void do_ls(char *);
void show_file_info( char *filename, struct stat *info);
void mode_to_letters(int mode, char str[]);
char *uid_to_name( uid_t uid);
char *gid_to_name(gid_t gid);

int main(int ac, char *av[]) 
{
    if ( ac == 1)
    {
        do_ls(".");
    }
    else
    {
        while ( --ac )
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
    if ( ( p_dir = opendir( dirname ) ) == NULL )
    {
        fprintf(stderr, "ls02: cannot open %s\n", dirname);
    }
    else
    {
        while ( ( p_dirent = readdir( p_dir ) ) != NULL ) 
        {
            dostat( p_dirent->d_name );
        }
        closedir(p_dir);
    }
    
}

void dostat(char *filename)
{
    struct stat info;
    if ( stat(filename, &info) == -1)
    {
        perror( filename ); // say why
    }
    else
    {
        show_file_info( filename, &info );
    }
    
}

void show_file_info( char *filename, struct stat *info)
{
    char modestr[11];   // type bit + 9 permission bits + '\0'
    mode_to_letters(info->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int) info->st_nlink);
    printf("%-8s ", uid_to_name(info->st_uid));
    printf("%-8s ", gid_to_name(info->st_gid));
    printf("%8ld ", (long)info->st_size);
    printf("%.12s ", 4+ctime(&info->st_mtim.tv_sec) );
    printf("%s\n", filename);
}
/**
 * This function takes a mode value and a char array
 * and puts the file type and the nine leters that 
 * correspond to the bits in mode.
 * Note: It does not code setuid, setgid, and sticky codes
*/
void mode_to_letters(int mode, char str[])
{
    strcpy(str, "----------");

    if ( S_ISDIR(mode) )    str[0] = 'd';
    if ( S_ISCHR(mode) )    str[0] = 'c';
    if ( S_ISBLK(mode) )    str[0] = 'b';

    if ( mode & S_IRUSR )   str[1] = 'r';
    if ( mode & S_IWUSR )   str[2] = 'w';
    if ( mode & S_IXUSR )   str[3] = 'x';

    if ( mode & S_IRGRP )   str[4] = 'r';
    if ( mode & S_IWGRP )    str[5] = 'w';
    if ( mode & S_IXGRP )   str[6] = 'x';

    if ( mode & S_IROTH)    str[7] = 'r';
    if ( mode & S_IWOTH)    str[8] = 'w';
    if ( mode & S_IXOTH)    str[9] = 'x';
}

#include <pwd.h>
char *uid_to_name( uid_t uid)
{
    struct passwd *getpwuid(), *p_pw;
    static char numstr[10];      // shared with gid_to_name()
    if ( ( p_pw = getpwuid( uid ) ) == NULL) // false, return string of uid
    {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
    {
        return p_pw->pw_name;
    }
    
}

#include <grp.h>
char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(), *p_grp;
    static char numstr[10];

    if ( (p_grp = getgrgid(gid)) == NULL)
    {
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
    {
        return p_grp->gr_name;
    }
    
}