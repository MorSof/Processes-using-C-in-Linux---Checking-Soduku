CC=gcc
OBJS=ChildCheck.o v1.o v2.o
EXEC=v1 ChildCheck v2
CFLAGS = -std=c99  
all: $(EXEC)

v1: v1.o
	$(CC) -o $@ $^

ChildCheck: ChildCheck.o
	$(CC) -o $@ $^

v2: v2.o

v1.c: v1.h
ChildCheck.c: ChildCheck.h
v2.c: v2.h

clean: 
	rm -f $(EXEC) $(OBJS)
	

