#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consoleLocation.h"

void showMenu(){
    CLEAR();
    printf("   =====Welcome=====\n");
    MOVERIGHT(3);
    printf("1:Login\n");
    MOVERIGHT(3);
    printf("2:Regisit\n");
    MOVERIGHT(3);
    printf("3:Exit\n");
}


int main(){
    HIDE_CURSOR();
    showMenu();

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
            CLEAR();
            
            if(choice==3){
                printf("~Bye~\n");
                SHOW_CURSOR();
                setDisplayMode(0,1);
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


