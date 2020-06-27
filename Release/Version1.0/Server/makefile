RM = rm -rf
OBJS = menu.o menuCollectLogin.o menuLogin.o clientValidate.o scanKeyboard.o checkID.o
FLAG = -pthread
TARGET = menu

$(TARGET):$(OBJS)
	gcc -o $(TARGET) $(OBJS) $(FLAG)
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

clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJS)
