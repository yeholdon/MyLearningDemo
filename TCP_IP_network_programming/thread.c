/**
 * author: yeholdon
 * email: yeholdon@gmail.com
 * date: 2019.7.4
 * theme: 《TCP/IP 网络编程》的线程编程
 */

/**
 * 要点：
 * 1. linux中，通过调用pthread_create函数创建子线程
 * 在创建子线程之前，运行的程序只是一个进程，而创建了子线程之后，
 * 原来的进程可以看成是这个进程中的主线程。创建子线程的时候会
 * 通过参数引用返回子进程ID同时调用子线程的main函数。而当主线程
 * 终止返回时，无论子线程状态如何，都会一起被终止。
 * 2. 这就需要控制好主线程的终止时间，解决这个问题可以在主线程创建子线程
 * 之后调用pthread_join函数，让调用它的线程即主线程进入等待状态
 * 等待子线程的结束，并得到通过参数引用得到子线程的返回值。所以这个
 * 函数的参数就是1）要等待结束的子线程的ID 2）子线程返回值的引用。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep
#include <string.h>
#include <pthread.h>

void *thread_main(void *arg) {
    int cnt = *((int *) arg);
    // 用来存返回值的空间
    char *msg = (char *)malloc(sizeof(char) * 50);
    strcpy(msg, "Hello, I'am thread~ \n");

    for (int i = 0; i < cnt; i++)
    {
        sleep(1);
        puts("running thread");
    }
    return (void *) msg;
    // 返回的时候又以void*返回
}

int main(int argc, char *argv[]) {
    pthread_t t_id; // 子线程ID
    int thread_param = 5; // 传给子线程的参数，实际传入的是其地址
    void *thr_ret; // 子进程返回值，存的是返回值的地址

    if (pthread_create(&t_id, NULL, thread_main, (void *)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    if (pthread_join(t_id, &thr_ret) != 0)
    {
        puts("pthread_join() error");
        return -1;
    }
    
    printf("Thread return meaasge: %s \n", (char *)thr_ret);
    free(thr_ret); // 动态分配的，而且堆内存是线程共享的，所以可以子线程开辟，主线程释放
    return 0;
}

