/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的多进程服务器实现中信号与signal函数程序
 */

/**
 * 要点：
 * 1. 父进程不可能在子进程结束之前一直等wait或者witpid来销毁子进程
 * 2. 解决方案：使用信号处理，即向OS求助，让OS在某些特定事件产生（比如这里的子进程终止）时
 * 向进程发送消息，并调用该消息对应的处理函数。这里的处理函数就可以是销毁子进程的函数。
 * 而让OS能够完成上面服务的请求通过signal注册函数完成。它的两个参数分别为信号类型，以及要帮忙执行的函数
 * 就相当于建立一个映射，让OS知道在什么时候帮什么忙。
 * 如果这里每指定对应要帮忙调用的函数，那么当该信号产生时，OS因为没有函数要调用，就会结束当前进程。
 * 3. 调用处理函数的是OS，但是OS也必须在该函数所属进程处于运行（非阻塞）状态时才能调用处理函数
 * 因此，产生信号时，为了调用相应处理函数，OS就要先唤醒这个可能已经阻塞的进程（比如这里阻塞在了sleep函数），而一旦被
 * 唤醒，即使在处理函数返回后，也不会再进入刚刚被唤醒时的阻塞状态，而是直接往后执行当前进程。所以sleep就可以被提前结束
 * 4. 利用上面的思想，就可以用来编写销毁僵尸进程的代码。只需设置产生子进程终止信号SIGCHILD时的处理函数代码为销毁子进程
 * 的相关操作即可。
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM)
    {
        puts("TIme out!");
    }
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT) 
    {
        puts("CTRL+C pressed");
    }
}

int main(int argc, char *argv[]) {
    // 向OS注册信号和对应的signal函数
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    
    alarm(2); // 产生SIGALARM

    for (int  i = 0; i < 3; i++)
    {
        puts("wait...");
        sleep(100);
    }
    
    return 0;
}