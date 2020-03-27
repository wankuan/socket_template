
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc , char *argv[])
{
    printf("sever is running!\n");
	struct sockaddr_in servaddr, cliaddr;
	int listenfd, connfd;

    /* 获得套接字描述符 */
	if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) < 0){
		perror("ERROR");
		exit(1);
	}
	memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    /* 接收指定端口的所有消息 */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8888);

    char *addr_s;
    uint16_t port;
    /* 将整形地址转为字符型 */
    addr_s = inet_ntoa(servaddr.sin_addr);
    /* 网络字节序转换 */
    port = ntohs(servaddr.sin_port);
    printf("local host:%s, port:%u\n",addr_s, port);

    /* 将套接字与地址结构绑定 */
	if(bind(listenfd , (struct sockaddr*)&servaddr , sizeof(servaddr)) < 0){
		perror("ERROR");
		exit(1);
	}
    /* 允许最大连接数1024 */
    /* 将主动套接字转为被动套接字，指使内核可以接收该套接字的请求 */
	if(listen(listenfd, 1024) < 0){
		perror("ERROR");
		exit(1);
	}

    uint16_t clilen = sizeof(cliaddr);
    /* 返回已连接的套接字（已完成TCP三次握手），完成服务时，将会被关闭
    ** 第一个参数为监听套接字，在整个服务器生命周期一直存在，仅一个，用于监听新TCP连接
    ** 第二、三参数为已连接TCP请求的客户端信息，可设为空指针
    */
    if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, (socklen_t*)&clilen)) < 0 ){
        perror("accept error");
        exit(1);
    }

    addr_s = inet_ntoa(cliaddr.sin_addr);
    port = ntohs(cliaddr.sin_port);
    printf("remote host:%s, port:%u\n",addr_s, port);


    ssize_t n;
    char buf[1000];
    /* 接收消息，read会一直阻塞，知道套接字接收到新消息或者被关闭，被关闭返回值为0
    ** 如果接收缓冲区设置过小，下一次消息来临时，会造成“粘包”现象
    */
    while((n = read(connfd, buf, sizeof(buf)))>0)
    {
        buf[n]='\0';
        /* 接收消息，并打印出长度 */
        printf("mesg:%s\n", buf);
        printf("len:%ld\n", n);
    }
    /* 客户端已关闭，关闭已连接的套接字 */
    close(connfd);
    /* 监听套接字关闭，服务器关闭 */
	close(listenfd);
}