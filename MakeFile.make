database: Server Client
Server: Server.c    LinkedList.c
	gcc -o Server Server.c LinkedList.c -I
Client: Client.c
	gcc -o Client Client.c
