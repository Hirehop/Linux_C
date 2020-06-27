// 服务器端处理函数
// 服务器端需要提供转储功能
// 单用户转发
// 群组转发


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
#include <semaphore.h>
#define BACKLOG 2
#define PORT 10086

sem_t mutex;

// 维护所有在线用户
// 采用文件实现：onlineUser.db
struct onlineUser{
    char id[12];
    char gid[12];
    int socket;
}ol;

// 在线用户集合
struct onlineUser ols[100];
int num=0;


/*************************************************************/
// 根据socket写入文件
// 同时写入ols
int insertOnlineUser(FILE *pfile){
    strcpy(ols[num].id,ol.id);
    strcpy(ols[num].gid,ol.gid);
    ols[num].socket = ol.socket;
    num++;

    int no = ol.socket;
    int res;
    res = fseek(pfile,no*sizeof(ol),SEEK_SET);
    if(res==0){
        res = fwrite(&ol,sizeof(ol),1,pfile);
    }
    return res;
}



// 根据socket删除相应数据
int deleteOnlineUser(FILE *pfile){
    memset(&ols[num],'\0',sizeof(ol));
    num--;

    int no = ol.socket;
    int res;
    res = fseek(pfile,no*sizeof(ol),SEEK_SET);
    memset(&ol,'\0',sizeof(ol));
    if(res==0){
        res = fwrite(&ol,sizeof(ol),1,pfile);
    }
    return res;
}

// 根据id查询socket
int selectOnlineUser(char *id){
    int i=0;
    int tempSocket=-1;
    for(;i<num;i++){
        if(strcmp(id,ols[i].id)==0){
            tempSocket = ols[i].socket;
        }
    }
    return tempSocket;
}
/*************************************************************/


/*************************************************************/
// 处理私聊消息
void *pthFunctionResv(void *arg){
	int sc = *(int*)arg;
    struct message mess;
    read(sc,&mess,sizeof(mess));
    if(mess.command==1){
        FILE *pfile;
        pfile = fopen("onlineUser.db","r+");
        sem_init(&mutex,0,1);
    	/*用户在线，写入文件*/
    	sem_wait(&mutex);
    	strcpy(ol.id,mess.thisId);
    	strcpy(ol.gid,mess.groupId);
    	ol.socket = sc;
    	insertOnlineUser(pfile);
    	sem_post(&mutex);
    	
        for(;;){
            read(sc,&mess,sizeof(mess));
    
            // 监测用户退出指令
            if(strncmp(mess.oppoId,"exit",4)==0){
                // 将该消息回送至客户端，客户端根据该消息进行终止
                write(sc,&mess,sizeof(mess));
                // 涉及到全局变量，信号量互斥
                sem_wait(&mutex);
                deleteOnlineUser(pfile);
                sem_post(&mutex);
                pthread_exit(NULL);
            }

            char thisId[12];
            char oppoId[12];
            char thisName[12];
            char data[128];
            memset(thisId,'\0',sizeof(thisId));
            memset(oppoId,'\0',sizeof(oppoId));
            memset(thisName,'\0',sizeof(thisName));
            memset(data,'\0',sizeof(data));
            strcpy(thisId,mess.thisId);
            strcpy(thisName,mess.thisName);
            strcpy(oppoId,mess.oppoId);
            strcpy(data,mess.data);

            memset(mess.data,'\0',mess.dataLength);
            strcpy(mess.data,data);
        
            // 服务器端将数据进行转发
            // 首先查找用户在线文件，匹配id获得socket
            int tempSocket = selectOnlineUser(oppoId);

            if(tempSocket!=-1){
                write(tempSocket,&mess,sizeof(mess));
            }else{
                strcpy(mess.thisId,oppoId);
                strcpy(mess.data,"not online");
                mess.dataLength = 10;
                // 原路返回对方不在线信息
                write(sc,&mess,sizeof(mess));
                // 服务器端测试输出
                printf("[%s]:not online\n",oppoId);
            }
        }
    }else if(mess.command==2){
            FILE *pfile;
            pfile = fopen("onlineUser.db","r+");
            sem_init(&mutex,0,1);
    	    /*用户在线，写入文件*/
    	    sem_wait(&mutex);
    	    strcpy(ol.id,mess.thisId);
    	    strcpy(ol.gid,mess.groupId);
    	    ol.socket = sc;
    	    insertOnlineUser(pfile);
    	    sem_post(&mutex);
    	    
    	    char groupId[6];
    	    memset(groupId,'\0',sizeof(groupId));
    	    strcpy(groupId,mess.groupId);
    	    
            for(;;){
                read(sc,&mess,sizeof(mess));
                // 监测用户退出指令
                if(strncmp(mess.oppoId,"exit",4)==0){
                    // 将该消息回送至指定客户端，客户端根据该消息进行终止
                    write(sc,&mess,sizeof(mess));
                    // 涉及到全局变量，信号量互斥
                    sem_wait(&mutex);
                    deleteOnlineUser(pfile);
                    sem_post(&mutex);
                    // 关闭该线程
                    pthread_exit(NULL);
                }
                // 获得信息中的groupId
                char groupId[12];
                strcpy(groupId,mess.groupId);
                // 查询所有当前组的用户，并向其分发消息
                int i=0;
                for(;i<num;i++){
                    if(strcmp(groupId,ols[i].gid)==0){
                         write(ols[i].socket,&mess,sizeof(mess));
                    }
                }
            }
        }
}

/*************************************************************/


/*************************************************************/
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
		pthread_create(&tid,NULL,pthFunctionResv,(void*)&sc);
    }
    
    return 0;
}
