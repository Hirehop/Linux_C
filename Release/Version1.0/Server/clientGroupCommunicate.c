// 客户端分组通信
// 传输协议见myMessageProtocol.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "consoleLocation.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "myMessageProtocol.h"

#include <pthread.h>
#include <semaphore.h>
#define IP "127.0.0.1"
#define PORT 10086

/*************************************************************/
// 模块来源：
// https://blog.csdn.net/qq_42292831/article/details/106153250
/*
  成功返回实际值
  失败：
    -1:不能以0开头
    -2:格式有误
    -3:长度错误
 */
int check(char *data,int len){
    if(data==NULL){
        return -1; 
    }   
    int length=0;
    while(data[length]!='\0'){
        length++;
    }   
    int i=0;
    int j=0;
    if(data[0]==0){
        return -1; 
    }   
    for(;i<length;i++){
        if(data[i]<='9'&&data[i]>='0'){}else{
            return -2; 
        }
    }   
    // 长度判断
    if(length!=len){
        return -3; 
    }   
    return 0;
}
/*************************************************************/

// 多线程共享用户信息，现将协议体全局化，注意操作使用信号量约束
// 主函数与发送线程使用，接受线程使用内部消息
struct message localMessage;
sem_t mutex;

// 光标行标记
int lineResv = 4;
int lineSend = 4;


// 接收模块
void *pthResvFunction(void *arg){
    int ss = *(int*)arg;
    struct message mess;
    MOVETO(lineResv,30);
    printf("-----Receive-Data-----\n");
    for(;;){
        memset(&mess,'\0',sizeof(mess));
        read(ss,&mess,sizeof(mess));
        if(strncmp(mess.oppoId,"exit",4)==0){
            printf("Exited!\n");
            pthread_exit(NULL);
        }
        lineResv++;
        MOVETO(lineResv,30);
        printf("[%s]: ",mess.thisId);
        fflush(stdout);
        write(1,mess.data,mess.dataLength);
        fflush(stdout);
        
        // 光标回位发送区
        MOVETO(lineSend,0);
        printf("->");
        fflush(stdout);
    }
}

void *pthSendFunction(void *arg){
    // 明确将要发送什么信息
    // 信息将会被送往服务器，经由服务器进行转发
    // 信息包含command,socket,thisId,thisName,oppoId,[oppoName],dataLength,data
    char dataBuffer[256];
    int ss = *(int*)arg;
    MOVETO(lineSend,0);
    printf("-----Send-Area-----\n");
    
    for(;;){
        sem_wait(&mutex);
        // 写入命令与socket
        localMessage.command = 2;
        sem_post(&mutex);

        // 清空dataBuffer
        memset(dataBuffer,'\0',sizeof(dataBuffer));
                
        lineSend++;
        MOVETO(lineSend,0);
        printf("->");
        fflush(stdout);
        MOVETO(lineSend,2);
        read(0,dataBuffer,sizeof(dataBuffer));
        fflush(stdin);

        // exit退出
        if(strncmp(dataBuffer,"exit",4)==0){
            strcpy(localMessage.oppoId,"exit");
            write(ss,&localMessage,sizeof(localMessage));
            pthread_exit(NULL);
        }

        // 将数据存入协议体
        strcpy(localMessage.data,dataBuffer);

        sem_wait(&mutex);
        // -1防止发送最后的换行符
        localMessage.dataLength = strlen(localMessage.data)-1;
        sem_post(&mutex);
    
        // 发往服务器
        write(ss,&localMessage,sizeof(localMessage));
    }
}


// 分组通讯主程序
// 传入用户信息结构体
void clientGroupCommunicate(struct userInfo user){
    // 初始化信号量
    sem_init(&mutex,0,1);
    system("clear");
    MOVETO(2,0);
    char thisId[12];
    char thisName[12];
    
    // 组id
    char groupNumberBuffer[12];
    memset(groupNumberBuffer,'\0',sizeof(groupNumberBuffer));
    for(;;){
        printf("Please input the GID you want to join: ");
        fflush(stdout);
        scanf("%s",groupNumberBuffer);
        int res = check(groupNumberBuffer,6);
        if(res<0){
            printf("GID format error! length=6!\n");
            continue;
        }
        break;
    }
    system("clear");
    MOVETO(2,0);
    printf("============Join in success!============\n");
    printf("==========Current GID:[%s]==========\n",groupNumberBuffer);

    // 读取用户自身的id和username
    strcpy(thisId,user.id);
    strcpy(thisName,user.username);

    sem_wait(&mutex);
    // 将用户自身信息写入消息体
    strcpy(localMessage.thisId,thisId);
    strcpy(localMessage.thisName,thisName);
    memset(localMessage.groupId,'\0',12);
    strcpy(localMessage.groupId,groupNumberBuffer);
    localMessage.command = 2;
    sem_post(&mutex);

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
    

    // 预先将自身信息发往服务器
    // 供服务器处理在线用户信息
    write(ss,&localMessage,sizeof(localMessage));

    // 接收数据线程
    // 发送数据线程
    pthread_t pidResv;
    pthread_t pidSend;
    pthread_create(&pidResv,NULL,pthResvFunction,(void*)&ss);
    pthread_create(&pidSend,NULL,pthSendFunction,(void*)&ss);
    
    pthread_join(pidResv,NULL);
    pthread_join(pidSend,NULL);
    close(ss);
}


// 单元测试用例
// 将用户信息体传入通讯函数
int main(int argc, char **argv){
    struct userInfo user;
    strcpy(user.id,argv[1]);
    strcpy(user.username,argv[2]);
    clientGroupCommunicate(user);
    return 0;
}
