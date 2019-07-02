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
 */
