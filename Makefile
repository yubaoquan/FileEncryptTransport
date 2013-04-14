obj = bit.o encrypt.o server.o client.o main.o

all : main.exe clean

main.exe : $(obj)
	gcc -o main.exe $(obj) -lm

main.o : main.c server.h client.h 
	gcc -c main.c

server.o : server.c bit.h encrypt.h
	gcc -c server.c

client.o : client.c bit.h encrypt.h
	gcc -c client.c

bit.o : bit.h bit.c
	gcc -c bit.c

encrypt.o : encrypt.h encrypt.c bit.h
	gcc -c encrypt.c

.PHONY : clean all
clean : 
	rm -f *.o
