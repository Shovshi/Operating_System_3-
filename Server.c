#include "Server.h"  

int ipv4_tcp_server(int port)
{
    int sock, client_sock;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    ssize_t read_size;
    FILE *file;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1234);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("bind");
        return 1;
    }

    if (listen(sock, 5) < 0) 
    {
        perror("listen");
        return 1;
    }

    printf("Waiting for incoming connections...\n");

    socklen_t client_len = sizeof(client);
    client_sock = accept(sock, (struct sockaddr *)&client, &client_len);
    if (client_sock < 0) 
    {
        perror("accept");
        return 1;
    }

    printf("Accepted connection from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    if (send(client_sock, "Welcome to the server!\n", 23, 0) < 0) 
    {
        perror("send");
        return 1;
    }

    file = fopen("received_file", "w");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    while ((read_size = recv(client_sock, buffer, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, sizeof(char), read_size, file);
    }

    fclose(file);
    close(client_sock);
    close(sock);

    return 0;
}

int ipv4_udp_server(int port)
{
    int sock, client_sock, addr_len;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    ssize_t read_size;
    FILE *file;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) 
    {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("bind");
        return 1;
    }

    printf("Waiting for incoming connections...\n");

    addr_len = sizeof(client);
    memset(&client, 0, addr_len);

    if ((read_size = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &addr_len)) < 0) 
    {
        perror("recvfrom");
        return 1;
    }

    printf("Accepted connection from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    if (sendto(sock, "Welcome to the server!\n", 23, 0, (struct sockaddr *)&client, addr_len) < 0) 
    {
        perror("sendto");
        return 1;
    }

    file = fopen("received_file", "w");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    while ((read_size = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &addr_len)) > 0)
    {
        fwrite(buffer, sizeof(char), read_size, file);
    }

    fclose(file);
    close(sock);

    return 0;
}

int ipv6_udp_server(int port)
{
    int sock, client_sock, addr_len;
    struct sockaddr_in6 server, client;
    char buffer[BUFFER_SIZE];
    ssize_t read_size;
    FILE *file;

    sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock == -1) 
    {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(port);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("bind");
        return 1;
    }

    printf("Waiting for incoming connections...\n");

    addr_len = sizeof(client);
    memset(&client, 0, addr_len);

    if ((read_size = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &addr_len)) < 0) 
    {
        perror("recvfrom");
        return 1;
    }

    char client_addr_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(client.sin6_addr), client_addr_str, INET6_ADDRSTRLEN);

    printf("Accepted connection from %s:%d\n", client_addr_str, ntohs(client.sin6_port));

    if (sendto(sock, "Welcome to the server!\n", 23, 0, (struct sockaddr *)&client, addr_len) < 0) 
    {
        perror("sendto");
        return 1;
    }

    file = fopen("received_file", "w");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    while ((read_size = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &addr_len)) > 0)
    {
        fwrite(buffer, sizeof(char), read_size, file);
    }

    fclose(file);
    close(sock);

    return 0;
}

int ipv6_tcp_server(int port)
{
    int sock, client_sock, addr_len;
    struct sockaddr_in6 server, client;
    char buffer[BUFFER_SIZE];
    ssize_t read_size;
    FILE *file;

    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(port);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("bind");
        return 1;
    }

    if (listen(sock, 1) < 0) 
    {
        perror("listen");
        return 1;
    }

    printf("Waiting for incoming connections...\n");

    addr_len = sizeof(client);
    memset(&client, 0, addr_len);

    if ((client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t *)&addr_len)) < 0) 
    {
        perror("accept");
        return 1;
    }

    char client_addr_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(client.sin6_addr), client_addr_str, INET6_ADDRSTRLEN);

    printf("Accepted connection from %s:%d\n", client_addr_str, ntohs(client.sin6_port));

    if (send(client_sock, "Welcome to the server!\n", 23, 0) < 0) 
    {
        perror("send");
        return 1;
    }

    file = fopen("received_file", "w");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    while ((read_size = recv(client_sock, buffer, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, sizeof(char), read_size, file);
    }

    fclose(file);
    close(client_sock);
    close(sock);

    return 0;
}

int uds_dgram_server()
{
    int s, s2, len;
    struct sockaddr_un remote, local = {
        .sun_family = AF_UNIX,
    };
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    for (;;) {
        printf("Waiting for a connection...\n");
        socklen_t slen = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &slen)) == -1) {
            perror("accept");
            exit(1);
        }

        printf("Connected.\n");

        // Read the filename from the client
        bytes_received = recv(s2, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv");
            exit(1);
        }

        // Open the file for writing
        FILE *fp = fopen(buffer, "wb");
        if (!fp) {
            perror("fopen");
            exit(1);
        }

        // Read the file data from the client and write it to the file
        while ((bytes_received = recv(s2, buffer, BUFFER_SIZE, 0)) > 0) 
        {
            if (fwrite(buffer, 1, bytes_received, fp) != (size_t)bytes_received) 
            {
                perror("fwrite");
                exit(1);
            }
        }

        if (bytes_received == -1) 
        {
            perror("recv");
            exit(1);
        }

        fclose(fp);
        printf("File received successfully.\n");

        close(s2);
    }

    return 0;
}

int uds_stream_server()
{
    int s, s2, len;
    struct sockaddr_un local, remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);

    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server listening...\n");

    socklen_t t = sizeof(remote);
    if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
        perror("accept");
        exit(1);
    }

    printf("Connection accepted.\n");

    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    size_t bytes_received;
    while ((bytes_received = recv(s2, str, sizeof(str), 0)) > 0) {
        if (fwrite(str, sizeof(char), bytes_received, fp) < bytes_received) {
            perror("fwrite");
            exit(1);
        }
    }

    fclose(fp);
    close(s2);
    close(s);
    printf("File received successfully.\n");

    return 0;
}

int mmap_server()
{
    int fd;
    void* addr;
    struct stat sb;

    // Open the file for writing
    if ((fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the shared memory object for reading
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Get the size of the shared memory object
    if (fstat(shm_fd, &sb) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory object into the address space of the calling process
    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Write the contents of the shared memory object to the file
    if (write(fd, addr, sb.st_size) != sb.st_size) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Unmap the shared memory object
    if (munmap(addr, sb.st_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    // Close the file and the shared memory object
    close(fd);
    close(shm_fd);

    // Remove the shared memory object
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
}

int pipe_server()
{
    int fd;
    int pipe_fd;
    char buf[BUFSIZ];
    ssize_t num_read;

    // Open the pipe for reading
    if ((pipe_fd = open(PIPE_NAME, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the file for writing
    if ((fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read from the pipe and write to the file
    while ((num_read = read(pipe_fd, buf, BUFSIZ)) > 0) {
        if (write(fd, buf, num_read) != num_read) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file and the pipe
    close(fd);
    close(pipe_fd);
}


int server_options(int argc, char* argv[])
{
    // Set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    // Set up server address
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket to address");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 1) < 0)
    {
        perror("Error listening for incoming connections");
        exit(1);
    }

    // Accept incoming connection
    int newsockfd, valread;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (newsockfd < 0)
    {
        perror("Error accepting incoming connection");
        exit(1);
    }

    // Receive data from client
   char *type, *param;
    while (1)
    {
        valread = read(newsockfd, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            break;
        }

        // Parse received data
        type = strtok(buffer, " ");
        param = strtok(NULL, " ");
    }


    // Close sockets
    close(newsockfd);
    close(sockfd);

    int port;
    port = atoi(argv[3]);

    
    send_options_server(type, param, port);
    return 0;
}

void send_options_server(char *type , char *param , int port)
{
    switch (type[0])
    {
    case 'i':
        if (strcmp(type, "ipv4") == 0) {
            switch (param[0]) {
                case 't':
                    ipv4_tcp_server(port);
                    break;
                case 'u':
                    ipv4_udp_server(port);
                    break;
            }
        } else if (strcmp(type, "ipv6") == 0) {
            switch (param[0]) {
                case 't':
                    ipv6_tcp_server(port);
                    break;
                case 'u':
                    ipv6_udp_server(port);
                    break;
            }
        } else {
            printf("Invalid option\n");
        }
        break;
    case 'm':
        mmap_server();
        break;
    case 'p':
        pipe_server();
        break;
    case 'u':
        switch (param[0])
        {
            case 'd':
                uds_dgram_server();
                break;
            case 's':
                uds_stream_server();
                break;
        }
        break;
    default:
        printf("Invalid option\n");
        break;
    }
}

    //ipv4_tcp_server(atoi(argc[1]));
    //ipv4_udp_server(atoi(argc[1]));
    //ipv6_udp_server(atoi(argc[1]));
    //ipv6_tcp_server(atoi(argc[1]));
    //uds_dgram_server();
    //uds_stream_server();
    //mmap_server();
    //pipe_server();