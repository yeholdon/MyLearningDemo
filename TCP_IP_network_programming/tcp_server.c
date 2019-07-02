/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.1
 * theme: 《TCP/IP 网络编程》的服务器端程序
 */

/**
 * 要点：
 * 1. listen的作用是转换套接字类型，主动转为被动
 *      即转成监听套接字。第二个参数维护的是连接队列长度
 * 
 */


#include <stdio.h>
#include <stdlib.h> // for exit()
#include <string.h>
#include <unistd.h> // for read()
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {

    // 变量定义，套接字描述符、地址结构体
    int serv_sock; // 监听套接字
    int clnt_sock; // 对应accept的客户端套接字
    // 服务器地址结构体
    struct sockaddr_in serv_addr;
    // accept的客户端套接字
    struct sockaddr_in clnt_addr;
    
    socklen_t clnt_addr_size;
    // 发送缓冲区
    char message[] = "Hello world!";
    // 判断用法是否正确
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 1. 创建服务器监听套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {   // 创建套接字失败
        error_handling("socket() error!");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // 不限定接入的网络接口(多网卡的情况)
    serv_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    // 2. 调用bind函数分配IP和端口号
    if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)  
    {
        error_handling("bind() error!");
    }
    // 3. 调用listen()将监听套接字转为可接收连接状态，
    // 默认为主动套接字，转成被动套接字，表示从内核接受指向该套接字的理由
    if (listen(serv_sock, 5) == -1) // 第二个参数为已完成三次握手的和未完成的两个连接队列的总长
    {
        error_handling("listen() error!");
    }

    // 4. 调用accept() 接收连接请求，阻塞在这个函数，直到有连接才返回服务器端与客户端对应的套接字描述符
    clnt_addr_size = sizeof(clnt_addr); // 因为accept的第三参数需要为地址，不能直接为返回值
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
    {
        error_handling("accept() error!");
    }

    // 5. 连接已建立，调用write函数将数据写入套接字发送
    write(clnt_sock, message, sizeof(message));

    // 6. 通信结束，调用close关闭相关套接字和连接
    close(clnt_sock);
    close(serv_sock);

    return 0;
}