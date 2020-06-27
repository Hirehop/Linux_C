RM = rm -rf
OBJS = serverValidate.o checkUser.o
FLAG = -pthread -L /usr/local/mysql/lib -l mysqlclient
TARGET = serverValidate

$(TARGET):$(OBJS)
	gcc -o $(TARGET) $(OBJS) $(FLAG)
serverValidate.o:serverValidate.c
	gcc -c serverValidate.c
checkUser.o:checkUser.c
	gcc -c checkUser.c

clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJS)
