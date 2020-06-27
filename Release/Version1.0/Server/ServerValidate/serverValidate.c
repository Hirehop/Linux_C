#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "myMessageProtocol.h"
 
#include <pthread.h>
#define BACKLOG 2
#define PORT 10085


// checkUser.o
// 服务器端验证函数声明
int checkUser(char*,char*,char*);

void server_process(int sc){
    struct userInfo user;
    read(sc,&user,sizeof(user));

    // [测试]输出接受信息
    printf("%s\n",user.id);
    // printf("%s\n",user.username);
    printf("%s\n",user.password);

    // 访问数据库查询数据
    // checkUser.o
    int res = checkUser(user.id,user.password,user.username);
    if(res==0){
        write(sc,&user,sizeof(user));
    }else{
        strcpy(user.username,"");
        write(sc,&user,sizeof(user));
    }
}

void *pthFunction(void *arg){
	int sc = *(int*)arg;
    server_process(sc);
}

int main(int argc,char **argv){
    int ss,sc;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int err;
    pthread_t tid;
 
    ss = socket(AF_INET,SOCK_STREAM,0);
    if(ss == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
 
    err = bind(ss,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(err == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }
    err = listen(ss,BACKLOG);
    if(err == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
 
    for(;;){
        int addr_len = sizeof(struct sockaddr);
        sc = accept(ss,(struct sockaddr*)&client_addr,&addr_len);
        if(sc == -1){
            if(errno = EINTR){
                continue;
            }
            else{
                perror("accept");
                exit(EXIT_FAILURE);
            }
        }
		pthread_create(&tid,NULL,pthFunction,(void*)&sc);
    }

    return 0;
}
 
