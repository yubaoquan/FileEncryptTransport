objs = bit.o encrypt.o server.o
objc = bit.o encrypt.o client.o

all : s.exe c.exe

s.exe : $(objs)
	gcc -o s.exe $(objs) -lm

c.exe : $(objc)
	gcc -o c.exe $(objc) -lm

server.o : server.c bit.h encrypt.h
	gcc -c server.c

client.o : client.c bit.h encrypt.h
	gcc -c client.c

bit.o : bit.h bit.c
	gcc -c bit.c

encrypt.o : encrypt.h encrypt.c bit.h
	gcc  encrypt.c

.PHONY : clean all
clean : 
	-rm edit $(objs) $(objc)