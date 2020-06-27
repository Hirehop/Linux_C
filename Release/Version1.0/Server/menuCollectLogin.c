/*
 该函数需要引用scanKeyboard.o和checkID.o
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "consoleLocation.h"
#include "myMessageProtocol.h"

// checkID.o中的check方法
int check(char*,int);

// scanKeyboard.o中的两个方法
int setDisplayMode(int, int);
int scanKeyboard();

/*
 返回收集到的信息[struct userInfo]
 */
struct userInfo showMenuCollectLogin(){
    struct userInfo user;
    system("clear");    
    MOVETO(2,0);
    printf("Please input your userID and password:\n");

    for(;;){
        MOVETO(4,0);
        printf("userID  :");
        MOVETO(4,11);

        // 开启回显
        setDisplayMode(0,1);
        scanf("%s",user.id);
    
        /* id检测模块 */
        // checkID.o中的check方法
        int res = check(user.id,9);
        if(res==-1){
            MOVETO(4,11);
            CLEAR_LINE();
            MOVETO(4,25);
            printf("[format error: 0 can not be the first one]");
        }else if(res==-2){
            MOVETO(4,11);
            CLEAR_LINE();
            MOVETO(4,25);
            printf("[format error: require all number]");
        }else if(res==-3){
            MOVETO(4,11);
            CLEAR_LINE();
            MOVETO(4,25);
            printf("[length error: length must be 9]");
        }else{
            break;
        }
    }
    MOVETO(4,25);
    CLEAR_LINE();

    // 关闭回显
    setDisplayMode(0,0);
    MOVETO(5,0);
    printf("password:");
    MOVETO(5,11);
    scanf("%s",user.password);
    // 吸收回车(!)
    getchar();
    // scanKeyboard();
    return user;
}

/*
int main(){
    showMenu();
    printf("%s\n",user.id);
    printf("%s\n",user.password);
    return 0;
}
*/
