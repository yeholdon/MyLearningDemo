/**
 * spwd.c - a simplified pwd version
 * starts in current directory and recursively
 * climbs up to root, prints top part then prints current part
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h> // for chdir
#include <string.h>
#include <stdlib.h>

ino_t   get_inode(char *);
void print_path_to(ino_t);
void inum_to_name(ino_t, char *, int);

int main()
{
    print_path_to(get_inode("."));  // print path to here
    putchar('\n');                                    
    return 0;
}

// prints path leading down to an object with this inode
void print_path_to(ino_t this_inode)
{
    ino_t   my_inode;
    char its_name[BUFSIZ];  // default buffer size (8192)
    if (get_inode("..") != this_inode)
    {
        chdir("..");                                                                                //up one dir
        inum_to_name(this_inode, its_name, BUFSIZ);     // get its name
        my_inode = get_inode(".");                                              // print head
        print_path_to(my_inode);                                                // recursively
        printf("/%s", its_name);                
    }
    
}

// look through current dir for a file with this inode number
// abd copies its name into buflen
void inum_to_name(ino_t inode_num , char * name_buf, int buf_len)
{
    DIR *p_dir;
    struct dirent *p_dirent;
    p_dir  = opendir(".");
    if (p_dir == NULL)
    {
        perror(".");
        exit(1);
    }
    // search dir 
    while ((p_dirent = readdir(p_dir)) != NULL )
    {
        if (p_dirent->d_ino == inode_num)
        {
            strncpy(name_buf, p_dirent->d_name, buf_len);
            name_buf[buf_len - 1] = '\0';
            closedir(p_dir);
            return;
        }
    }
    fprintf(stderr, "error looking for inum %ld\n", inode_num);
    exit(1);
    
}

ino_t get_inode(char *filename)
{
    struct stat info;
    if (stat(filename, &info) == -1)
    {
        fprintf(stderr, "Cannot stat");
        perror(filename);
        exit(1);
    }
    return info.st_ino;   
}