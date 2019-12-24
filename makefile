
all:server client

server:server.c
	gcc -g -o server server.c -l wsock32 
client:client.c
	gcc -g -o client client.c -l wsock32 
clean:
	rm -rf *~ *.o *.exe