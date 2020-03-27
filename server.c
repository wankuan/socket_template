
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

	socklen_t clilen;

	if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) < 0){
		perror("socket error");
		exit(1);
	}
	memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(clilen));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8000);

	if(bind(listenfd , (struct sockaddr*)&servaddr , sizeof(servaddr)) < 0){
		perror("bind error");
		exit(1);
	}

	if(listen(listenfd , 666) < 0){
		perror("listen error");
		exit(1);
	}

    clilen = sizeof(cliaddr);
    if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0 ){
        perror("accept error");
        exit(1);
    }

    char *addr_s = inet_ntoa(cliaddr.sin_addr);
    uint16_t port = ntohs(cliaddr.sin_port);
    printf("remote host:%s, port:%u\n",addr_s, port);

    addr_s = inet_ntoa(servaddr.sin_addr);
    port = ntohs(servaddr.sin_port);
    printf("local host:%s, port:%u\n",addr_s, port);

    ssize_t n;
    char buff[1000];

    while((n = read(connfd, buff, 1000))>0)
    {
        buff[n]='\0';
        printf("mesg:%s\n", buff);
        printf("len:%ld\n", n);
    }
    close(connfd);
	close(listenfd);
}