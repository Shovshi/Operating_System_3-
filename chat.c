#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void usage() 
{
    printf("Usage: stnc [-c IP PORT] [-s PORT]\n");
}

int main(int argc, char *argv[]) 
{
    int server_sock, client_sock;
    struct sockaddr_in server, client;
    char message[1024], response[1024];

    if (argc != 3 && argc != 4) 
    {
        usage();
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) 
    {
        if (argc != 4) 
        {
            usage();
            return 1;
        }

        client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (client_sock == -1) 
        {
            perror("socket");
            return 1;
        }

        server.sin_addr.s_addr = inet_addr(argv[2]);
        server.sin_family = AF_INET;
        server.sin_port = htons(atoi(argv[3]));

        if (connect(client_sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
        {
            perror("connect");
            return 1;
        }

        if (recv(client_sock, response, 1024, 0) < 0) 
        {
            perror("recv");
            return 1;
        }
        printf("%s\n", response);

        while (1) 
        {
            printf("Enter message: ");
            fgets(message, 1024, stdin);

            if (send(client_sock, message, strlen(message), 0) < 0) 
            {
                perror("send");
                return 1;
            }

            if (recv(client_sock, response, 1024, 0) < 0) 
            {
                perror("recv");
                return 1;
            }
            printf("Received: %s\n", response);
        }

        close(client_sock);

    } 
    else if (strcmp(argv[1], "-s") == 0) 
{
    if (argc != 3) 
    {
        usage();
        return 1;
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) 
    {
        perror("socket");
        return 1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("bind");
        return 1;
    }

    if (listen(server_sock, 1) < 0) 
    {
        perror("listen");
        return 1;
    }
    printf("Listening on port %s...\n", argv[2]);

    socklen_t client_size = sizeof(client);
    client_sock = accept(server_sock, (struct sockaddr *)&client, &client_size);
    if (client_sock < 0)
    {
        perror("accept");
        return 1;
    }

    if (send(client_sock, "Connected to client.", strlen("Connected to client."), 0) < 0) 
    {
        perror("send");
        return 1;
    }

    while (1) 
    {
        printf("Enter message: ");
        fgets(message, 1024, stdin);

        if (send(client_sock, message, strlen(message), 0) < 0) 
        {
            perror("send");
            break;
        }

        if (recv(client_sock, message, 1024, 0) < 0) 
        {
            perror("recv");
            break;
        }
        printf("Received: %s", message);
    }

    close(client_sock);
}

return 0;
}