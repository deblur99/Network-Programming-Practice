all : 03_client 03_server

CC = gcc

03_client : 03_client.o
	gcc -o 03_client 03_client.o

03_server : 03_server.o linked_list.o
	gcc -o 03_server 03_server.o linked_list.o

03_client.o : 03_client.c
	gcc -c 03_client.c

03_server.o : 03_server.c
	gcc -c 03_server.c 

linked_list.o : linked_list.c
	gcc -c linked_list.c 

clean :
	rm *.o 03_client 03_server