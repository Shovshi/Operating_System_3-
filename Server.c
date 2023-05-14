#include "Server.h"  


unsigned int checksum(char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length);
    if (!data) {
        printf("Error: could not allocate memory\n");
        fclose(file);
        return 0;
    }

    fread(data, 1, length, file);
    fclose(file);

    unsigned int sum = 0;
    int i;
    for (i = 0; i < length; i++) {
        sum += data[i];
    }

    free(data);
    return sum;
}

void send_options_server(char *type , char *param , int port , int qFlag)
{
   if (strcmp(type, "ipv4") == 0 && (strcmp(param, "tcp")) == 0)
    {
        ipv4_tcp_server(port, qFlag);
    }
    else if (strcmp(type, "ipv4") == 0 && (strcmp(param, "udp")) == 0)
    {
        ipv4_udp_server(port, qFlag);
    }
    else if (strcmp(type, "ipv6") == 0 && (strcmp(param, "tcp")) == 0)
    {

        ipv6_tcp_server(port, qFlag);
    }
    else if (strcmp(type, "ipv6") == 0 && (strcmp(param, "udp")) == 0)
    {

        ipv6_udp_server(port, qFlag);
    }

    else if (strcmp(type, "mmap") == 0 && (strcmp(param, "filename")) == 0)
    {

        mmap_server(qFlag);
    }
    else if (strcmp(type, "pipe") == 0 && (strcmp(param, "filename")) == 0)
    {

        pipe_server(qFlag);
    }
    else if (strcmp(type, "uds") == 0 && (strcmp(param, "dgram")) == 0)
    {

        uds_dgram_server(qFlag);
    }
    else if (strcmp(type, "uds") == 0 && (strcmp(param, "stream")) == 0)
    {

        uds_stream_server(qFlag);
    }
}

int ipv4_tcp_server(int port , int qFlag)
{
    int server_socket, client_socket, bytes_received, bytes_sent;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;
    char buffer[BUFFER_SIZE];
    FILE *fp;
    
    // create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        return -1;
    }

    // set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket");
        return -1;
    }

    // listen for incoming connections
    listen(server_socket, 1);
    printf("Listening on port %d...\n", port);

    // accept the client connection
    client_address_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_length);
    if (client_socket < 0) {
        perror("Error accepting client connection");
        return -1;
    }


    // open a file for writing
    fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        perror("Error opening file for writing");
        return -1;
    }

    // receive the file data from the client
    while (bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0))
    {
        if (bytes_received < 0) {
            perror("Error receiving file data");
            return -1;
        }
        bytes_sent = fwrite(buffer, sizeof(char), bytes_received, fp);
        if (bytes_sent < bytes_received) {
            perror("Error writing to file");
            return -1;
        }
    }
    fclose(fp);

    printf("File received and saved as received_file.txt\n");

    // close the client socket and server socket
    close(client_socket);
    close(server_socket);

     if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }

    return 0;
}

int ipv4_udp_server(int port , int qFlag)
{
     int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[BUFFER_SIZE];
    FILE *fp;

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return -1;
    }

    // setup server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // bind socket to address
    if (bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        return -1;
    }

    // create output file
    if ((fp = fopen(FILENAME, "wb")) == NULL) {
        perror("file creation failed");
        return -1;
    }

    // receive file contents from client
    len = sizeof(cliaddr);
    while ((n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cliaddr, &len)) > 0) {
        fwrite(buffer, 1, n, fp);
    }

    // close file and socket
    fclose(fp);
    close(sockfd);

     if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }

    return 0;
}

int ipv6_udp_server(int port , int qFlag)
{
   int sockfd;
    struct sockaddr_in6 server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    FILE* fp;

    // Create socket
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(port);
    server_addr.sin6_addr = in6addr_any;

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        return -1;
    }

    // Open file for writing
    fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Receive data from client and write to file
    socklen_t client_len;
    int bytes_received;
    while (1) {
        // Receive data from client
        client_len = sizeof(client_addr);
        bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &client_addr, &client_len);
        if (bytes_received < 0) {
            perror("Error receiving data");
            fclose(fp);
            return -1;
        }

        // If no more data is received, break out of loop
        if (bytes_received == 0) {
            break;
        }

        // Write data to file
        int bytes_written = fwrite(buffer, 1, bytes_received, fp);
        if (bytes_written < bytes_received) {
            perror("Error writing to file");
            fclose(fp);
            return -1;
        }
    }

    // Close file and socket
    fclose(fp);
    close(sockfd);

    if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }


    return 0;
}

int ipv6_tcp_server(int port , int qFlag)
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

    file = fopen(FILENAME, "w");
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

    if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }


    return 0;
}

int uds_dgram_server(int qFlag)
{
    // Create a socket for receiving datagrams
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket() failed");
        return;
    }

    // Bind the socket to a local address
    struct sockaddr_un local_addr = {0};
    local_addr.sun_family = AF_UNIX;
    strncpy(local_addr.sun_path, SERVER_PATH, sizeof(local_addr.sun_path) - 1);
    unlink(local_addr.sun_path);
    if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind() failed");
        close(sockfd);
        return;
    }

    // Open the output file
    FILE* file = fopen(FILENAME, "wb");
    if (!file) {
        perror("fopen() failed");
        close(sockfd);
        return;
    }

    // Receive datagrams and write them to the output file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_received, file);
        if (bytes_written < bytes_received) {
            perror("fwrite() failed");
            break;
        }
    }

    // Close the file and socket
    fclose(file);
    close(sockfd);

    if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }


    return 0;
}

int uds_stream_server(int qFlag)
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

    if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }


    return 0;
}

int mmap_server(int qFlag)
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

    if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }

}

int pipe_server(int qFlag)
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

    if (qFlag == 0)
    {
        // Calculate the checksum
        unsigned int check_sum = checksum("received_file.txt");
        unsigned int checksum_client = checksum("large_file.txt");
        printf("Checksum of sent file: %u\n", checksum_client);
        printf("Checksum of received file: %u\n", check_sum);
    }

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
    port = atoi(argv[2]);

    int qFlag = 0;
    if (argc >= 4 && strcmp(argv[3], "-q") == 0)
    {
        qFlag = 1;
    }
    send_options_server(type, param, port , qFlag);
    return 0;
} 
