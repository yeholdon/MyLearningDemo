/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的半关闭客户端程序
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

const int BUF_SIZE = 30;
void error_handling(char * message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    // 1. 准备缓冲区，存地址的数据结构等
    int sd;
    FILE *fp;

    struct sockaddr_in serv_addr; // 存IP地址，端口号的结构体
    char buf[BUF_SIZE]; // 消息接受缓冲区
    int read_cnt = 0; // 读取的数据总长度

    fp = fopen("receive.dat", "wb");
    sd = socket(PF_INET, SOCK_STREAM, 0);

    if (sd == -1)
    {
        error_handling("socket() error!");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if ((connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) == -1)
    {
        error_handling("connect() error!");
    }

    while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
    {
        fwrite((void *)buf, 1, read_cnt, fp);
    }

    puts("Received file data");
    write(sd, "Thank you", 10);
    fclose(fp);
    close(sd);
    return 0;
}