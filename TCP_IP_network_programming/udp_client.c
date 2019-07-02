
/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的UDP客户器端程序
 */

/**
 * 要点：
 * 1. 连续用sendto函数发送三条消息
 * 2. 已连接的和未连接的UDP套接字
 * TCP套接字需要先调用connect函数注册目标地址，但UDP无需注册
 * 但是UDP如何确定发送目标地址？答案很简单，就是每次sendto发送都给出目标地址。
 * 每次sendto分为三个阶段：向UDP套接字注册目标地址，传输数据，删除套接字中注册的地址信息
 * 这样做灵活性好，可以每次sendto都发给不同的人，但是这也就意味着，在需要连续多次
 * 向同一个人发送消息的时候每次都得注册地址信息然后又删除，这样重复注册过于浪费。
 * 3. 解决方法就是：也给TCP套接字维护一个连接，即注册了目标地址的套接字就是已连接的套接字。
 * 同样也是调用connect函数进行注册，只不过这里不是要与对方UDP套接字连接，而只是注册地址信息。
 * 注册了之后可以用sendto\recvfrom也可以用write\read
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
    char msg1[] = "Hi!";
    char msg2[] = "I'm another UDP host!";
    char msg3[] = "Nice to meet you!";

    struct sockaddr_in serv_addr;
    socklen_t addr_size;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        errorHanding("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    // conect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    sendto(sock, msg2, sizeof(msg2), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    sendto(sock, msg3, sizeof(msg3), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    close(sock);
    return 0;
}