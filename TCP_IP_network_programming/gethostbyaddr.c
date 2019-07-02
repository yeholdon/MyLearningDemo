/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的IP获取域名程序
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>


void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    struct hostent *host;
    struct sockaddr_in addr;

    if (argc != 2)
    {
        printf("Usage : %s <addr>\n", argv[0]);
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    // 1. 含有IP地址信息的in_addr结构体指针。2. 向第一个参数传递的地址信息的字节数，
    // IPv4为4，IPv6为16. 3. 地址族信息
    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
    if (!host)
    {
        errorHandling("gethost... error!");
    }

    printf("Official name:%s \n", host->h_name);
    for (int i = 0; host->h_aliases[i]; i++)
    {
        printf("Alliases %d: %s \n", i + 1, host->h_aliases[i]);
    }

    printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

    for (int i = 0; host->h_addr_list[i]; i++)
    {
        // 结构体里的字符串转成in_addr再用, inet_ntoa转成字符串，这样顺带解决字节序问题。
        printf("IP addr %d: %s \n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
        // printf("IP addr %d: %s \n", i + 1, host->h_addr_list[i]);
        // 不能直接输出，因为这里面存的不是IP地址，而是指向in_addr结构体变量的指针。而且是char *型的，而不是in_addr *
        // 这是因为in_addr只能存IPv4地址，而这个结构体要支持多种网络地址，比如IPv6地址信息也要支持，所以就干脆存
        // char *型，再显式转换成相应类型。其实当指针对象不明确时用void *最好，但是这里是因为定义这些函数的时候void *还没标准化
    }
    return 0;
}