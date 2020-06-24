#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>

#include "myMessageProtocol.h"

// 执行sql语句
// 返回 0/-1
int executeSql(MYSQL *conn,char * sql)
{
	if(mysql_real_query(conn,sql,strlen(sql))){
		return -1;
    }
	return 0;
}

// 向数据库表中写入数据(执行DML语句)
// 返回 0/-1
int addUser(struct userInfo user){
    // 初始化链接句柄
    MYSQL *conn;
    conn = mysql_init(NULL);
    if(conn==NULL){
        printf("conn_init error\n");
        exit(-1);
    }
    // 进行实际链接
    conn = mysql_real_connect(conn,"localhost","root","root","mydb1",0,0,0);
    if(conn){
        printf("connect success\n");
    }else{
        printf("connect error\n");
    }
    // 执行SQL语句
    char id[12];
    char username[12];
    char password[12];
    memset(id,'\0',sizeof(id));
    memset(username,'\0',sizeof(username));
    memset(password,'\0',sizeof(password));
    strcpy(id,user.id);
    strcpy(username,user.username);
    strcpy(password,user.password);
    char sql[128] = "insert into user values(";
    strcat(sql,id);
    strcat(sql,",'");
    strcat(sql,username);
    strcat(sql,"','");
    strcat(sql,password);
    strcat(sql,"');");
    
    int res = executeSql(conn,sql);
    return res;
}


// 测试
/*
int main(){
    struct userInfo user;
    strcpy(user.id,"179074002");
    strcpy(user.username,"xiaoB");
    strcpy(user.password,"179074002");
    int res = addUser(user);
    printf("%d\n",res);
    return 0;
}
*/
