// 本地通讯
// IP地址使用本地环回口IP:127.0.0.1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define IP "127.0.0.1"
#define PORT 10085

struct userInfo{
    char id[12];
    char username[12];
    char password[12];
};


// 参数一：传输文件描述符
// 参数二：用户传入结构体，内部封装了用户的id和password，发往服务端进行验证
// 返回值：userInfo结构体
//    成功user.username为实际值
//    失败user.username为""
struct userInfo client_process(int ss,struct userInfo user){
    write(ss,&user,sizeof(user));
    read(ss,&user,sizeof(user));
    return user;
}


// 验证处理主函数
// 参数：用户传入的userInfo结构体
// 返回值：
//     userInfo结构体[依赖内部处理函数client_process]
struct userInfo clientValidate(struct userInfo user){
    int ss;
    struct sockaddr_in server_addr;
 
    ss = socket(AF_INET,SOCK_STREAM,0);
    if(ss == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);
 
    connect(ss,(struct sockaddr*)&server_addr,sizeof(server_addr));

    user = client_process(ss,user);
    close(ss);
    return user;
}
