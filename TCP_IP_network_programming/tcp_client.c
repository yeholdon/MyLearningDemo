/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.1
 * theme: 《TCP/IP 网络编程》的客户端程序
 */


/** 要点：
 * 1. TCP：提供可靠的、按顺序传输的、基于字节的面向连接的数据传输方式
 * 同时传输的数据不存在数据边界，套接字和连接一一对应。
 * 2. 注意点：read和write的调用次数并无太大意义，因为不存在边界数据，
 * 接受缓冲区满后发送端就会停止传输
 * 3. inet_addr将点分十进制的IP地址字符串转成32位整数，并转成网络序
 *    inet_aton()则可以直接传入字符串和sockaddr_in结构体，省去额外赋值
 *    inet_itoa()相反。注意它存转换后IP地址的缓冲区在每次调用该函数后会覆盖
 *    函数返回的就是该缓冲区的首地址，要记得复制里面的内容出去。
 * 4. connect在服务器端接收连接请求的时候返回，这时并不意味着服务器端调用了
 *      accept函数，只是说明服务器端把请求信息记录到了等待队列。所以connect
 *      函数返回后并不立即进行数据交换。
 * 5. 客户端套接字的地址信息在哪？ 是在调用connect函数时由内核分配，IP就是主机IP
 *      端口随机。无需用bind绑定
 */
 

#include <stdio.h> // for stderr and so on
#include <stdlib.h>
#include <string.h> // for memset
#include <unistd.h>
#include <arpa/inet.h>  // for strcut sockaddr_in, 嵌套包含了socket.h
#include <sys/socket.h>
void error_handling(char * message);

int main(int argc, char *argv[]) {
    // 1. 准备缓冲区，存地址的数据结构等
    int sock;
    struct sockaddr_in serv_addr; // 存IP地址，端口号的结构体
    char message[30]; // 消息接受缓冲区
    int str_len = 0; // 读取的数据总长度
    int idx = 0, read_len = 0; // 当前接受缓冲区的指针位置，每次读取的数据长度

    if (argc != 3)
    {
        // argv[0]存的是文件名
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    // 2. socket() 创建套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    // 套接字创建失败
    if (sock == -1)
    {
        error_handling("socket() error");
    }

    // 先清空结构体
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4协议族, PF_INET的#define
    // inet_addr转成32位整数的IP地址，并转成网络序
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    // 3. connect() 发出与服务器的连接
    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    {
        // 连接失败
        error_handling("connect() error!");
    }
    // 4. 循环读取服务器发来的数据，一次只读一个，返回值为读取的字节数，0则表示文件尾, read有阻塞和非阻塞两种
    // 在socket中的read默认在读不到数据的时候会阻塞，这时候是数据还没发来，但是读到文件尾的时候应该返回0.
    while (read_len = read(sock, &message[idx++], 1))
    {
        if (read_len == -1)
        {
            // 比如描述符有问题，I/O出错等
            error_handling("read() error!");
        }
        else
        {
            str_len += read_len;
        }
        
    }
    
    printf("Message from server: %s \n", message);
    printf("Fuction read call count: %d \n", str_len);
    close(sock);
    return 0;
}



void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}