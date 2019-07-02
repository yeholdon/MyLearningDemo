
/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.1
 * theme: 《TCP/IP 网络编程》的echo客户器端改进程序
 */

/**
 * 要点：
 * 1. 循环发送直到输入Q
 * 2. 发了之后立即read，但很可能还没传回来，但是因为是死循环，所以很可能发生
 *   多次发的一起传回来的情况，这不是我们想要的。
 * 3. 或者给出的字符串太长，服务器write一次，但是TCP一个包传不完，客户端就有可能还没全部收到就调了read
 * 4. 这个程序改进了前面两点问题，即一次发送之后，循环读取，直到收到回射的所有数据为止
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 1024;
void errorHanding(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    // 定义相关变量
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_addr;
    int str_len;
    int recv_len, recv_cnt;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        errorHanding("socket() error!");
    }
    

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi((argv[2])));
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
    {
        errorHanding("connect() error!");
    }
    else
    {
        puts("Connected......");
    }
    
    while (1)
    {
        // 提示用户输入
        fputs("Input message(Q to quit): ", stdout);
        // 获取用户输入
        fgets(message, BUF_SIZE, stdin);
   
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
        {
            break;
        }
        // 发送
        recv_len = 0;
        
        str_len = write(sock, message, strlen(message));
        while (recv_len < str_len)
        {
           recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1); // 从revclen开始往下
           if (recv_cnt == -1)
           {
               errorHanding("read() error!");
           }
           recv_len += recv_cnt;
           
        }
        message[str_len] = 0; // 去掉末尾的'\n'
        printf("Message from server: %s", message);
    }
    close(sock);
    
    return 0;
}