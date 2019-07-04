/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.3
 * theme: 《TCP/IP 网络编程》的多进程服务器程序
 */

/**
 * 要点：
 * 1. 信号处理函数解决父进程需要一直等待子进程结束的问题
 * 2. waitpid解决销毁结束的子进程的问题
 * 3. 记住fork是复制进程，描述符也会复制从而造成一个套接字
 * 有多个描述符，因此要在进程中把多余的描述符关掉。
 */

#include <stdio.h> // for stderr and so on
#include <stdlib.h>
#include <string.h> // for memset
#include <unistd.h>
#include <arpa/inet.h>  // for strcut sockaddr_in, 嵌套包含了socket.h
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

const int BUF_SIZE = 30;

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("removed proc id: %d \n", pid);
    }
    
}

int main(int argc, char *argv[]) {
    int serv_sd, clnt_sd;
    struct sockaddr_in serv_addr, clnt_addr;

    pid_t pid;
    struct sigaction act;
    socklen_t addr_sz;
    int str_len, state;
    char buf[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }
    

    act.sa_handler = read_childproc; // 绑定处理函数
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    // socket
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bind() error!");
    }

    if (listen(serv_sd, 5) == -1)
    {
        error_handling("listen() error!");
    }

    while (1)
    {
        addr_sz = sizeof(clnt_addr);
        clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_addr, &addr_sz);
        if (clnt_sd == -1)
        {
            continue; // 当前连接失败，等待下一个连接
        }
        else
        {
            puts("new client connected...");
        }
        
        // 连接成功，创建子进程来维护该连接
        pid = fork();
        if (pid == -1)
        {
            close(clnt_sd);//创建子进程失败，当前连接也关闭掉，等待下一次连接
            continue;
        }

        if (pid == 0)
        {
            // 创建子进程成功，关掉顺带复制的监听描述符
            close(serv_sd); // 子进程只维护客户端描述符
            while ((str_len = read(clnt_sd, buf, BUF_SIZE)) != 0)
            {
                // 只有真的全部接受完才会返回0
                write(clnt_sd, buf, str_len);
            }
            // 接受完毕, 关闭连接
            close(clnt_sd);
            puts("client disconnected...");
            return 0; // 该子进程的连接处理完，关闭连接，返回
        }
        else
        {   // 对父进程来说，创建的客户端连接描述符没用，关闭
            // 这时因为一个套接字有多个描述符，关闭描述符并不会关闭套接字
            close(clnt_sd);
        }
        
        
    }
    // 应该是不会执行到，按照程序的逻辑
    close(serv_sd);
    return 0;
}