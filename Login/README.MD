### `介绍`
=> 本文件夹是完整的基于Socket客户服务器端验证程序


```
    1.数据位数检测模块：checkId.c
    2.监听键盘录入：scanKeyboard.c
    3.收集登录信息：menuCollectLogin.c
    4.menu.c -> menuCollectLogin.c -> clientValidate.c -> showMenuLogin.c
    5.认证客户端，将封装后的userInfo结构体传给服务器处理：clientValidate.c
        5.1 将一个仅包含id和password的userInfo结构体传给服务器
        5.1 读取服务器认证后返回一个完整的userInfo结构体，内部username有数据,失败为""
    6.认证客户端:serverValidate.c
        6.1 服务器认证后返回一个完整的userInfo结构体，内部username有数据,失败为""
    
    7.makefile
        RM = rm -rf
        OBJS = menu.o menuCollectLogin.o menuLogin.o clientValidate.o scanKeyboard.o checkID.o menuRegister.o 
        TARGET = menu

        $(TARGET):$(OBJS)
            gcc -o $(TARGET) $(OBJS)
        scanKeyboard.o:scanKeyboard.c
            gcc -c scanKeyboard.c
        checkID.o:checkID.c
            gcc -c checkID.c
        menu.o:menu.c
            gcc -c menu.c
        menuCollectLogin.o:menuCollectLogin.c
            gcc -c menuCollectLogin.c
        menuLogin.o:menuLogin.c
            gcc -c menuLogin.c
        clientValidate.o:clientValidate.c
            gcc -c clientValidate.c
        menuRegister.o:menuRegister.c
            gcc -c menuRegister.c

        clean:
            -$(RM) $(TARGET)
            -$(RM) $(OBJS)



    Date:2020-06-24 Wednesday
``
