CC=gcc
OBJ= Client.o Server.o chat.o main.o
NAME = stnc
CFLAGS = -Wall -g

all : stnc

stnc: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

main.o: main.c Client.h Server.h chat.h
	$(CC) $(CFLAGS) -c $< -o main.o

Server.o: Server.c Server.h
	$(CC) $(CFLAGS) -c $< -o Server.o

Client.o: Client.c Client.h
	$(CC) $(CFLAGS) -c $< -o Client.o

chat.o: chat.c chat.h
	$(CC) $(CFLAGS) -c $< -o chat.o 

.PHONY = clean all 

clean:
	rm -f *.o *.a $(NAME)