/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的多进程服务器实现中信号与sigaction函数程序
 */

/**
 * sigaction就是signal的替代与加强版，而且signal在不同版本*nix系统中
 * 可能会有区别，但sigaction完全相同。
 * 这里的struct sigaction不知道为啥插件识别不出来，但编译没问题
 */

#include <stdio.h>
#include <unistd.h> // 存放了fork()、pipe()、read()等unix系统调用
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM)
    {
        puts("Time out!");
    }
    alarm(2);
}

int main(int argc, char *argv[]) {
    struct sigaction act;
    // 指定信号相关的选项和特性，这里用不到，为0即可
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
    alarm(2);
    for (int i = 0; i < 3; i++)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}