CC=gcc
OBJ= client_b.o server_b.o chat.o main.o
NAME = stnc
CFLAGS = -Wall -g

all : stnc

stnc: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

main.o: main.c client_b.h server_b.h chat.h
	$(CC) $(CFLAGS) -c $< -o main.o

server_b.o: server_b.c server_b.h
	$(CC) $(CFLAGS) -c $< -o server_b.o

client_b.o: client_b.c client_b.h
	$(CC) $(CFLAGS) -c $< -o client_b.o

chat.o: chat.c chat.h
	$(CC) $(CFLAGS) -c $< -o chat.o 

.PHONY = clean all 

clean:
	rm -f *.o *.a $(NAME)