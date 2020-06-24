#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>

int main(){
    // 初始化链接句柄
    MYSQL *conn;
    int res;
    MYSQL_RES *res_ptr;
    MYSQL_FIELD *field;
    MYSQL_ROW result_row;
    int row,column;
    int i,j;
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

    char *sql = "select * from stu;";
    res = mysql_query(conn,sql);
    if(res!=0) {
        perror("my_query");
        mysql_close(conn);
        exit(0);
    } else{
        //把查询结果给res_ptr
        res_ptr = mysql_store_result(conn);
        //如果结果不为空,则输出
        if(res_ptr) {
            column = mysql_num_fields(res_ptr);
            row = mysql_num_rows(res_ptr);
            printf("查到%d行\n",row);
            //输出结果的字段名
            for(i = 0;field = mysql_fetch_field(res_ptr);i++) {
                printf("%10s",field->name);
            }
        }
        puts("");
        //按行输出结果
        for(i = 1;i < row+1;i++){
            result_row = mysql_fetch_row(res_ptr);
            for(j = 0;j< column;j++) {
                printf("%10s",result_row[j]);
            }
            printf("\n");
        }
        puts("");
    }
    mysql_close(conn);
    return 0;
}
