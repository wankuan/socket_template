#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main(int argc, char *argv[])
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8000);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    const char buffer[3][100]={"1st message","2nd message","3rd message"};
    int mesg_len = sizeof(buffer)/sizeof(buffer[0]);

    for(int i=0; i<mesg_len; i++){
        write(sock, buffer[i], strlen(buffer[i]));
        sleep(1);
    }
    // if(shutdown(sock,SHUT_RDWR) != 0){
    //     printf("error shutdown\n");
    // }
    close(sock);
    return 0;
}