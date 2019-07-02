/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.1
 * theme: 《TCP/IP 网络编程》的echo服务器端程序
 */

/**
 * 要点：
 * 1. 循环五次调用accept接收连接，每次处理后关闭
 * 2. 以字符串为单位接收信息并read后立即write
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 1024;
void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    // 1. 定义套接字、地址、缓冲区等变量
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    // 2. socket()
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        errorHandling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    // 2. bind()
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        errorHandling("bind error!");
    }

    // 3. listen()
    if (listen(serv_sock, 5) == -1)
    {
        errorHandling("listen() error!");
    }

    // 4. accept() 依次接收5个客户端的连接。注意不是同时，同时的话要多线程
    clnt_addr_size = sizeof(clnt_addr);
    for (int i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
        {
            errorHandling("accept() error");
        }
        else
        {
            printf("Connected client %d \n", i + 1);
        }
        
        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
        {
            // 边收边发, 收几个发几个
            write(clnt_sock, message, str_len);
        }
        
        close(clnt_sock);
    }
    // 关闭监听套接字连接
    close(serv_sock);
    
    return 0;
}