/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的套接字选项程序
 */

/**
 * 要点：套接字有很多特性，可以通过可选项进行更改。
 * 套接字可选项是分层的。IPPROTO_IP层可选项是IP协议相关事项，IPPROTO_TCP层可选项
 * 是TCP协议相关事项， SOL_SOCKET层是套接字相关通用可选项
 * 1. 要设置这些可选项，最基本的要提供相应的get和set方法：getsockopt & setsockopt这两个函数
 * 2. 下面这个程序就是用SO_TYPE可选项查看套接字类型，1为TCP套接字，2为UDP套接字。该选项为只读
 * 3. 至于getsockopt的用法是一样的，就不写了
 * 4. 其他常用的套接字选项有：SO_SNDBUF(发送缓冲区大小)、SO_RCVBUF(接收缓冲区大小)、SO_REUSEADDR(重用处于Time_wait状态的套接字)、TCP_NODELAY(无需停等ack再发下一个)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int tcp_sd, udp_sd;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sd = socket(PF_INET, SOCK_STREAM, 0);
    udp_sd = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM: %d \n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);

    state = getsockopt(tcp_sd, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if (state)
    {   // 成功返回0，失败返回-1
        errorHandling("getsocket() error!");
    }
    printf("Socket type one: %d \n", sock_type);

    state = getsockopt(udp_sd, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if (state)
    {
        errorHandling("getsockopt() error!");
    }
    printf("Socket type two: %d \n", sock_type);
    
    return 0;
}