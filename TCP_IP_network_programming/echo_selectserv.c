/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.3
 * theme: 《TCP/IP 网络编程》的多进程服务器程序,用管道把回声字符串传给另一个进程负责保存数据
 */

/**
 * 要点：
 * 1. select是用在accept之前的
 * 2. 在调用select之前，要先把要关心的套接字的描述符在fd_set中的相应位置1
 * 表示监视这个描述符，select将可监视的描述符分为3种，分别为是否有待读数据、可传输无阻塞数据以及
 * 是否发生异常，分别由其第2\3\4个参数传入。
 * 3. 监视的描述符集合中有描述符发生变化时（比如第一类描述符集合里有描述符有待读数据），select
 * 函数就会返回，并将fd_set变量中没有发生变化的位都置0（当然前提是已注册），这时候再遍历这些位，
 * 找出仍为1的位，就能找出当前有待读数据的描述符，然后进行数据读取或者别的操作（比如监听描述符可以调用accept
 * 函数来创建新的连接，这时要记得将得到的新描述符注册
 * 4. 另外，要是一直都没有描述符状态变化，那select就会一直阻塞，这时就可以设置一个计时器，时间到的时候还没有变化的
 * 描述符的话，select就会返回，此时返回值为0，用以区别正常返回时返回值（表示变化的描述符个数）
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

const int BUF_SIZE = 100;

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serv_sd, clnt_sd;
    struct sockaddr_in ser_addr, clnt_addr;
    struct timeval timeout;
    fd_set reads, cp_reads;

    socklen_t addr_sz;
    int fd_max, str_len, fd_num; // 用来存select的返回值
    char buf[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sd == -1)
    {
        error_handling("socket() error!");
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sd, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) == -1)
    {
        error_handling("bind() error");
    }

    if (listen(serv_sd, 5) == -1)
    {
        error_handling("liten() error!");
    }


    FD_ZERO(&reads);
    FD_SET(serv_sd, &reads);
    fd_max = serv_sd;
    
    while (1)
    {
        // 设置好监听后复制一份fdset，
        // 把被select改的和原始的分开
        cp_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        
        if ((fd_num = select(fd_max + 1, &cp_reads, 0, 0, &timeout)) == -1)
        {
            // 少了fdnum外的括号的话fd_num永远位为0
            // 发生3错误，，返回-1
            break;
        }

        if (fd_num == 0)
        {
            // 这次时间到，直接进入下一次循环
            continue;
        }
        
        // 有变化的描述符
        // 遍历描述符，找出变化的
        for (int i = 0; i < fd_max + 1; i++)
        {
            if (FD_ISSET(i, &cp_reads))
            {
                // 描述符i变化了
                if (i == serv_sd)
                {
                    // 为监听描述符
                    // 说明有新连接请求
                    addr_sz = sizeof(clnt_addr);
                    clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_addr, &addr_sz);
                    // 接受连接后立即设置相应监听位
                    FD_SET(clnt_sd, &reads);
                    if (fd_max < clnt_sd)
                    {   // 新连接的描述符大于原来的最大描述符，则更新最大描述符
                        fd_max = clnt_sd;
                    }
                    printf("connected client: %d \n", clnt_sd);
                }
                else
                {
                    // 为客户端对应描述符
                    // 读取数据
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0)
                    {
                        // 读完了，到达文件尾
                        // 关闭连接前记得将监听位置0
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    }
                    else
                    {
                        // 正常读取，回传
                        write(i, buf, str_len);
                    }
                    
                    
                }
                
                
            }
            
        }
        
        
    }
    close(serv_sd);
    return 0;
    
}