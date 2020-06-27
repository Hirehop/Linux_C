RM = rm -rf
OBJS = serverCommunicate.o
FLAG = -pthread
TARGET = serverCommunicate

$(TARGET):$(OBJS)
	gcc -o $(TARGET) $(OBJS) $(FLAG)
serverCommunicate.o:serverCommunicate.c
	gcc -c serverCommunicate.c

clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJS)
