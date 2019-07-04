
/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.3
 * theme: 《TCP/IP 网络编程》的进程间通信
 */

/**
 * 要点：进程间通信
 * 1. 进程间通信运用管道来进行，一个管道其实就是一个半双工通信
 * 的信道，要实现全双工通信，使用两个管道即可。
 * 2. 管道的参数是一个用来存放两个文件描述符的长度为2的数组。
 * 第0个元素存的是读的描述符，第1个元素是写的描述符
 */
#include <stdio.h>
#include <unistd.h>

const int BUF_SIZE = 30;

int main(int argc, char *argv[]) {
    int fds1[2], fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    __pid_t pid;
    // 创建两个管道，实现进程间全双工通信，一个管道只能实现半双工
    pipe(fds1), pipe(fds2);
    pid = fork();
    if (pid == 0)
    {
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, BUF_SIZE);
        printf("Child proc output: %s \n", buf);
    }
    else
    {
        read(fds1[0], buf, BUF_SIZE);
        write(fds2[1], str2, sizeof(str2));
        printf("Parent proc output: %s \n", buf);
        sleep(3); // 防止父进程结束时子进程还没运行完就把子进程也销毁了
    }
    return 0;
    
}