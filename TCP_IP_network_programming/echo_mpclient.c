
/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.3
 * theme: 《TCP/IP 网络编程》的多进程客户端程序，分割TCP的IO
 */

/**
 * 要点：
 * 1. 信号处理函数解决父进程需要一直等待子进程结束的问题
 * 2. waitpid解决销毁结束的子进程的问题
 * 3. 记住fork是复制进程，描述符也会复制从而造成一个套接字
 * 有多个描述符，因此要在进程中把多余的描述符关掉。
 * 4. 分割IO就是写和读分别用一个进程，这样就可以同时读和写，无需写完再读
 * 提高效率。echo因为从应用需求上就是写完才能读，所以这里其实没必要分割IO
 * 这里用分割IO只是为了说明和练习这种方法
 * 5. read()在字节流套接字上的特点，和普通文件IO不同，它默认是阻塞的，也就是说，当
 * 调用red但是输入缓冲区中没有数据可以读的话，read会阻塞，而不是返回，而只有遇到文件尾，
 * 也就是对方已经发送完毕的时候，才会返回0，否则返回的是这次读到的数据字节数，
 * 这个字节数可能比调用read时请求的要少，或者相等，这里的写法其实是不够完整的，这里默认了
 * 一次read就能够读完，但是如果一次读不完的话，就只能读出请求的大小，也就是buf的大小，第二次循环
 * 再读的时候，因为给的还是buf的首地址，就会把前一次读的覆盖,这样多次
 * 读取虽然可以接在一起但是不够自然。所以完美的用法应该是在这种情况下接收缓冲区
 * buf也要用一个指针指向当前已读到的内容的末尾，这样一次没读完下一次read就可以把读的内容接在第一次读的
 * 后面，而不会覆盖第一次读的。具体可以看udp中对read的包裹函数readn()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 30;

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_routine(int sock, char *buf) {
    while (1)
    {
        int str_len = read(sock, buf, BUF_SIZE);
        if (str_len == 0)
        {
            return;
        }
        buf[str_len] = 0; // 末尾'\0',缓冲区应该设为BUF_SIZE + 1才行，不然这里实际是超界的
        // 但是因为是函数形参传入，所以每报错
        printf("Message from server: %s", buf);
        
    }
    
}

void write_routine(int sock, char *buf) {
    while (1)
    {   // 先获取输入
        fgets(buf, BUF_SIZE, stdin);
        if (!strcmp(buf, "Q\n") || !strcmp(buf, "q\n"))
        {
            // fgets会读入最后的'\n'
            // 这次连接发完了，半关闭连接
            shutdown(sock, SHUT_WR);
            return;
        }
        // 否则继续发
        write(sock, buf, BUF_SIZE);
    }
    
}

int main(int argc, char *argv[]) {
    int sd;
    char buf[BUF_SIZE + 1];
    pid_t pid;
    struct sockaddr_in serv_addr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        error_handling("socket() error!");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error!");
    }
    
    pid = fork();
    if (pid == 0)
    { // 子进程，专门负责写
        write_routine(sd, buf);
    }
    else
    {
        // 父进程，专门负责读
        read_routine(sd, buf);
    }
    
    
}