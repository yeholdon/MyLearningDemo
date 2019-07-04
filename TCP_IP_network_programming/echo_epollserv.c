/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.3
 * theme: 《TCP/IP 网络编程》的基于epoll的IO复用服务器程序
 */

/**
 * 要点：
 * 1. select的缺点：一、每次监视的套接字变化都得遍历所有描述符
 * 一个个找出产生变化的套接字对应的描述符。二、每次调用select都得重新传给
 * OS需要监视的fd_set，而传给操作系统的这个操作是消耗很大的（这也是select性能不佳
 * 的主要原因。
 * 2. epoll的出现就是为了解决select的以上不足
 * 3. epoll的三个函数分别对应了select的三个操作，只不过有了改进
 * 1）epoll_create对应创建fd_set变量，即创建文件描述符保存空间（称为”epoll例程“）
 * 2）epoll_ctl对应FD_SET和FD_CLR在epoll例程内部注册监视对象文件描述符
 * 3）epoll_wait对应select。区别就是它会自动把发送变化的描述符信息集合起来存入
 * epoll_event结构体数组中，这样就可以直接得到发送变化的描述符而无需遍历所有
 * 而且epoll例程只需创建一次无需每次重新传递给OS处理。
 * 4. 值得注意的是那个epoll_event结构体数组需要malloc动态分配
 */

#include <stdio.h> // for stderr and so on
#include <stdlib.h>
#include <string.h> // for memset
#include <unistd.h>
#include <arpa/inet.h>  // for strcut sockaddr_in, 嵌套包含了socket.h
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>

const int BUF_SIZE = 100;
const int EPOLL_SIZE = 30;

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {

    int serv_sd, clnt_sd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t addr_sz;
    int str_len;
    char buf[BUF_SIZE];

    // epoll变量
    struct epoll_event *ep_events;// 变化描述符集合
    struct epoll_event event; // epoll
    int epfd, event_cnt; // epoll例程描述符， 变化描述符个数

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

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bind() error");
    }

    if (listen(serv_sd, 5) == -1)
    {
        error_handling("liten() error!");
    }

    // epoll设置
    // 1.创建epoll例程空间
    epfd = epoll_create(EPOLL_SIZE);// 实际OS会忽略这个参数
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    // 无需强转malloc返回值指针类型

    // 2.生成注册用的描述符epoll_event结构
    event.events = EPOLLIN; // 监视事件类型
    event.data.fd = serv_sd; // 监视的描述符
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sd, &event); // 向os修改监视信息
    
    while (1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1); //-1表示阻塞直到有变化
        if (event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }

        for (int i = 0; i < event_cnt; i++)
        {
            // 虽然也是遍历，但是这里范围小很多，全是有变化的
            if (ep_events[i].data.fd == serv_sd)
            {
                // 变化的为监听描述符，有新连接
                addr_sz = sizeof(clnt_addr);
                clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_addr, &addr_sz);
                event.events = EPOLLIN;
                event.data.fd = clnt_sd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sd, &event);
                printf("connected client: %d \n", clnt_sd);
            }
            else
            {
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if (str_len == 0)
                {
                    // EOF
                    // 关闭连接
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("closed client: %d \n", ep_events[i].data.fd);
                }
                else
                {
                    // 正常读到
                    write(ep_events[i].data.fd, buf, str_len); // echo
                }
                
                
            }
            
            
        }
        
        
    }
    
    close(serv_sd);
    close(epfd);
    return 0;
}