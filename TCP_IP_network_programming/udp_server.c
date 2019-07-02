
/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的UDP服务器端程序
 */

/**
 * 要点：
 * 1. 调用3次recvfrom接收三条数据并打印。验证UDP存在数据边界，调用接收函数得到次数要和调用发送函数的次数一致
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 30;

void errorHanding(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t addr_size;
    int str_len;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        errorHanding("soxket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    // bind
    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        errorHanding("bind() error!");
    }
    
    for (int i = 0; i < 3; i++)
    {
        sleep(5);
        addr_size = sizeof(clnt_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&clnt_addr, &addr_size);
        // 传入的是发送方也就是客户端的地址结构，因为这里会自动填入，从而获取信息是从哪里来的
        printf("Message %d: %s \n", i + 1, message);
    }
    close(sock); // 全关闭（双向关闭）连接
    return 0;
}

