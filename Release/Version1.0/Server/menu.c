#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "consoleLocation.h"
#include "myMessageProtocol.h"

// 当前结构体被包含在myMessageProtocol.h中
/*
struct userInfo{
    char id[12];
    char username[12];
    char password[12];
};
*/

// menuCollectLogin.o
struct userInfo showMenuCollectLogin();

// menuLogin.o
int showMenuLogin();

// clientValidate.o
struct userInfo clientValidate(struct userInfo);

// menuRegister.o
struct userInfo showMenuRegister();

// scanKeyboard.o
int setDisplayMode(int,int);
int scanKeyboard();

void showMenu(){
    system("clear");
    MOVETO(2,0);
    printf("   =====Welcome=====\n");
    MOVERIGHT(3);
    printf("1:Login\n");
    MOVERIGHT(3);
    printf("2:Info\n");
    MOVERIGHT(3);
    printf("3:Exit\n");
}


int main(){
    struct userInfo user;
    begin:
    HIDE_CURSOR();
    showMenu();
    
    int choice = 1;
    int line = 3;
    int res = 0;
    MOVETO(line,0);
    printf("->\n");

    for(;;){
        setDisplayMode(0,0);
        res = scanKeyboard();
        switch(res){
            case 119:
                if(line<=3){
                    line = 3;
                    choice = 1;
                }else{
                    line--;
                    choice--;
                }
                MOVETO(2,0);
                showMenu();
                MOVETO(line,0);
                printf("->\n");
                break;
            case 115:
                if(line>=5){
                    line=5;
                    choice = 3;
                }else{
                    line++;
                    choice++;
                }
                MOVETO(2,0);
                showMenu();
                MOVETO(line,0);
                printf("->\n");
                break;
            default:
                MOVETO(2,0);
        }
        if(res==10){
            /* ... */
            if(choice==1){
                memset(&user,'\0',sizeof(struct userInfo));
                // menuCollectLogin.o中的showMenuCollectLogin方法
                user = showMenuCollectLogin();

                /*
                // 发往服务器验证
                // 简单测试
                if(strcmp(user.id,"179074001")==0 && strcmp(user.password,"179074001")==0){

                    // 取得username
                    strcpy(user.username,"xiaoA");
                    printf("\n\n[ Login success! ]\n");
                    sleep(1);
                    // menuLogin.o中的showLoginMenu方法
                    showMenuLogin(user);
                }else{
                    // 提示登录失败
                    printf("\n\n[ userID/password error, 3s back! ]\n");
                    sleep(3);
                    goto begin;
                }
                */

                // clientValidate.o
                // 用户验证
                // 验证成功后user结构体中username字段将被填充实际值，否则填充""
                // 完整user将作为参数传入登录后功能菜单页
                user = clientValidate(user);

                if(strcmp(user.username,"")==0){
                    // 提示登录失败
                    printf("\n\n[ userID/password error, 3s back! ]\n");
                    sleep(3);
                    goto begin;
                }else{
                    printf("\n\n[ Login success! ]\n");
                    sleep(1);
                    // menuLogin.o中的showLoginMenu方法
                    showMenuLogin(user);
                }

            }else if(choice==2){
                system("clear");
                MOVETO(2,0);
                printf("=====Program Information=====\n");
                printf("     1. Author:Krystal X.\n");
                printf("     2. Version:1.0\n");
                printf("     3. ^_^\n");
                printf("\n\n[Enter to get back]\n");
                for(;;){
                    int temp = scanKeyboard();
                    if(temp==10){
                        break;
                    }
                }
                goto begin;
            }else if(choice==3){
                printf("~Bye~\n");
                SHOW_CURSOR();
                setDisplayMode(0,1);
                system("clear");
                return 0;
            }else{
                printf("choice:%d\n",choice);
                printf("[Enter to get back]\n");
                for(;;){
                    int temp = scanKeyboard();
                    if(temp==10){
                        break;
                    }
                }
            }   
            MOVETO(2,0);
            showMenu();
            MOVETO(line,0);
            printf("->\n");
        }
    }
    SHOW_CURSOR();
    setDisplayMode(0,1);
    return 0;
}


