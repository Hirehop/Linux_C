#include <stdio.h>

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
    if(data[0]=='0'){
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

/*
// 测试
int main(){
    char *data1 = "a123";
    char *data2 = "1a23";
    char *data3 = "123a";
    char *data4 = "123";
    
    printf("%d\n",check(data1,3));
    printf("%d\n",check(data2,3));
    printf("%d\n",check(data3,3));
    printf("%d\n",check(data4,3));
    printf("%d\n",check(data4,4));
    return 0;
}
*/
