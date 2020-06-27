/*
 　该头文件定义了用于用户数据通信协议+用户认证协议
 */



/*
 　用户认证协议，id唯一
 */
struct userInfo{
    char id[12];
    char username[12];
    char password[12];
};


// 用户可发送数据最大长度:128
// 
struct message{
    // 功能标记,用于指明是私聊还是群聊
    int command;
    // 用户socket套接字，类似seesion用户维护用户登录状态
    int socket;
    // 自身id和用户名
    char thisId[12];
    char thisName[12];
    // 对方id和用户名
    char oppoId[12];
    char oppoName[12];
    // 数据
    int dataLength;
    char data[128];
    // 组
    char groupId[12];
};
