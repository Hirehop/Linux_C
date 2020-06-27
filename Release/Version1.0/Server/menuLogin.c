#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "consoleLocation.h"
#include "myMessageProtocol.h"

// scanKeyboard.o中的两个方法
int setDisplayMode(int, int);
int scanKeyboard();

// requireIdToP2P.o
struct userInfo showP2PRequire();

// P2P通信客户端
void clientP2PCommunicate(struct userInfo);


void showLoginMenu_temp(char *username){
    // CLEAR();
    system("clear");
    char welcomeInfo[128];
    MOVETO(2,0);
    sprintf(welcomeInfo,"   =====Welcome[%s]=====\n",username);
    printf("%s",welcomeInfo);
    MOVERIGHT(3);
    printf("1:P2P Communication\n");
    MOVERIGHT(3);
    printf("2:Group Communication\n");
    MOVERIGHT(3);
    printf("3:Logout\n");
}


// 本方法中包含了当前用户信息，和待处理的对方用户信息
// 执行用户的选择
int showMenuLogin(struct userInfo user){
    aaa:
    HIDE_CURSOR();
    char username[12];
    strcpy(username,user.username);
    showLoginMenu_temp(username);

    int choice = 1;
    int line = 3;
    MOVETO(line,0);
    printf("->\n");

    setDisplayMode(0,0);

    for(;;){
        int res = scanKeyboard();
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
                showLoginMenu_temp(username);
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
                showLoginMenu_temp(username);
                MOVETO(line,0);
                printf("->\n");
                break;
            default:
                MOVETO(2,0);
        }
        if(res==10){
            if(choice==1){
                // 转到聊天界面
                system("clear");
                // 显示光标+按下回显
                setDisplayMode(0,1);
                SHOW_CURSOR();
                MOVETO(2,0);
                printf("==========Communicating==========\n");
                printf("Format:[oppositeId messageContent]\n");

                // 将本地用户登录信息传入通讯模块
                // clientP2PCommunicate(user);
                // 调用system执行
                char clientP2PProgram[128]="./clientP2P ";
                strcat(clientP2PProgram,user.id);
                strcat(clientP2PProgram," ");
                strcat(clientP2PProgram,user.username);
                strcat(clientP2PProgram,"\0");
                
                system(clientP2PProgram);
                goto aaa;
                
            }else if(choice==2){
                char clientGroupProgram[128]="./clientGroup ";
                strcat(clientGroupProgram,user.id);
                strcat(clientGroupProgram," ");
                strcat(clientGroupProgram,user.username);
                strcat(clientGroupProgram,"\0");
                
                system(clientGroupProgram);
                goto aaa;
            }else if(choice==3){
                return 0;
            }
        }
    }
}
