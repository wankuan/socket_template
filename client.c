#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main(int argc, char *argv[])
{
    /* 获得套接字描述符 */
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client_socket < 0){
		perror("ERROR");
		exit(1);
    }
    /* 套接字地址结构 */
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    /* 本地的8000端口 */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    /* 连接服务器 */
    if(connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		perror("ERROR");
		exit(1);
    }
    /* 三条消息 */
    const char buffer[3][100]={"1st message","2nd message","3rd message"};
    int mesg_len = sizeof(buffer)/sizeof(buffer[0]);

    /* 依次发送 */
    for(int i=0; i<mesg_len; i++){
        /* 分三次发送消息
        ** 如发送过快，服务器接收的消息将会黏在一起，俗称“粘包”，粘包原因：
        ** 1.连续发送数据时，tcp协议栈会将小内容拼接成大内容，一次性发送到服务器端，因此造成粘包
        ** 2.当发送内容较大时，由于服务器端的recv（buffer_size）方法中的buffer_size较小，
        ** 不能一次性完全接收全部内容，因此在下一次请求到达时，接收的内容依然是上一次没有完全接收完的内容
        */
        write(client_socket, buffer[i], strlen(buffer[i]));
        printf("mesg:%s\n", buffer[i]);
        sleep(1);
    }
    /* 关闭socket，关闭TCP连接 */
    close(client_socket);
    return 0;
}