/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.2
 * theme: 《TCP/IP 网络编程》的UDP服务器端程序
 */

/**
 * 要点：
 * 基于TCP的半关闭
 * 1. 什么是半关闭？ 用close函数关闭的连接是全关闭的，就是双向流都关闭了。
 * 2. 为什么要半关闭？ 比如服务器给客户端发一个文件，发完之后服务器完成了自己要发送
 * 给客户端的所有内容就会关闭连接，但是这时候如果客户端还有额外的信息要发送给服务器，
 * 由于服务器已经完全关闭了连接，那也就没办法再收到信息了。
 * 3. 半关闭的实现。断开输出流时向对方主机发送EOF，客户端通过函数返回值接受EOF，
 * 而不是把EOF和内容一起发送，避免和文件内容的冲突。具体实现是调用shutdown函数，其中
 * 第一个参数为套接字描述符，第二个参数可选关闭输入流、输出流还是两个都关闭。
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 30;

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serv_sd, clnt_sd;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    fp = fopen("file_server.c", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // 不限定接入的网络接口(多网卡的情况)
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    //  调用bind函数分配IP和端口号
    if (bind(serv_sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)  
    {
        error_handling("bind() error!");
    }
    // 调用listen()将监听套接字转为可接收连接状态，
    // 默认为主动套接字，转成被动套接字，表示从内核接受指向该套接字的理由
    if (listen(serv_sd, 5) == -1) // 第二个参数为已完成三次握手的和未完成的两个连接队列的总长
    {
        error_handling("listen() error!");
    }

    // 4. 调用accept() 接收连接请求，阻塞在这个函数，直到有连接才返回服务器端与客户端对应的套接字描述符
    clnt_addr_sz = sizeof(clnt_addr); // 因为accept的第三参数需要为地址，不能直接为返回值
    clnt_sd = accept(serv_sd, (struct sockaddr *) &clnt_addr, &clnt_addr_sz);
    if (clnt_sd == -1)
    {
        error_handling("accept() error!");
    }

    while (1)
    {
        // 从文件读BUF_SIZE个长度为1的数据出来到buf
        read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE)
        {
            write(clnt_sd, buf, read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
        
    }
    
    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd, buf, BUF_SIZE);
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;
}