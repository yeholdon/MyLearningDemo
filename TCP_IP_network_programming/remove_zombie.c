/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的销毁僵尸进程程序
 */

/**要点：
 * 用信号和处理函数使父进程阻塞的问题，然后在处理函数中使用waitpid函数
 * 来销毁子进程。
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig) {
    int status;
    // 三个参数：1.表示等待终止的目标子进程的ID，-1表示等待任意子进程终止
    // 2. 保存子进程返回值 3.表示每终止的子进程也不阻塞，而是返回0并退出
    // 这里因为用了信号，所以调用该函数时一定有终止的子进程
    __pid_t pid = waitpid(-1, &status, WNOHANG);
    // 子进程正常终止，返回真
    if (WIFEXITED(status))
    {
        printf("Removed proc id: %d \n", pid);
        // 打印子进程终止时返回的值
        printf("Child send: %d \n", WEXITSTATUS(status));
    }
    
    
}

int main(int argc, char *argv[]) {
    __pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc; // 绑定处理函数
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0)
    {   // 子进程
        puts("Hi! I'm child process");
        sleep(10);
        return 12;
    }
    else
    {
        // 父进程
        printf("Child proc id: %d \n", pid);
        pid = fork();
        if (pid == 0)
        {
            // 另一个子进程
            puts("Hi! I'm child process");
            sleep(10);
            exit(24);
        }
        else
        {
            printf("Child proc id: %d \n", pid);
            // 发生信号时（子进程终止时）父进程被唤醒，实际暂停时间无需25秒
            for (int i = 0; i < 5; i++)
            {
                puts("wait...");
                sleep(5);
            }
            
        }
        
        
    }
    
    
}