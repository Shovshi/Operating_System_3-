#include "Client.h"

int ipv4_tcp_client(int port, char *ip_address) 
{
    int sock;
    struct sockaddr_in server;
    char response[1024];
    char buffer[1024];
    int file_fd;
    ssize_t read_size;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        perror("socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(ip_address);
    server.sin_family = AF_INET;
    server.sin_port = htons(1234);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("connect");
        return 1;
    }

    if (recv(sock, response, 1024, 0) < 0) 
    {
        perror("recv");
        return 1;
    }
    printf("%s\n", response);

    file_fd = open(FILENAME, O_RDONLY);
    if (file_fd == -1)
    {
        perror("open");
        return 1;
    }

    while ((read_size = read(file_fd, buffer, 1024)) > 0)
    {
        if (send(sock, buffer, read_size, 0) < 0) 
        {
            perror("send");
            return 1;
        }
    }

    close(file_fd);
    close(sock);

    return 0;
}

int ipv4_udp_client(int port , char* ip_address)
{
    int sock;
    struct sockaddr_in server;
    char response[1024];
    char buffer[BUFFER_SIZE];
    int file_fd;
    ssize_t read_size;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) 
    {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_address);
    server.sin_port = htons(port);

    printf("Sending file to %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    file_fd = open(FILENAME, O_RDONLY);
    if (file_fd == -1)
    {
        perror("open");
        return 1;
    }

    while ((read_size = read(file_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (sendto(sock, buffer, read_size, 0, (struct sockaddr *)&server, sizeof(server)) < 0) 
        {
            perror("sendto");
            return 1;
        }
    }

    close(file_fd);
    close(sock);

    return 0;

}

int ipv6_udp_client(int port , char* ip_address)
{
    int sock;
    struct sockaddr_in6 server;
    char response[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int file_fd;
    ssize_t read_size;

    sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return 1;
    }

    server.sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, ip_address, &server.sin6_addr) <= 0)
    {
        perror("inet_pton");
        return 1;
    }
    server.sin6_port = htons(port);

    printf("Sending file to %s:%d\n", ip_address, ntohs(server.sin6_port));

    file_fd = open(FILENAME, O_RDONLY);
    if (file_fd == -1)
    {
        perror("open");
        return 1;
    }

    while ((read_size = read(file_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (sendto(sock, buffer, read_size, 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("sendto");
            return 1;
        }
    }

    close(file_fd);
    close(sock);

    return 0;
}

int ipv6_tcp_client(int port, char* ip_address) {
    int sock;
    struct sockaddr_in6 server;
    char buffer[BUFFER_SIZE];
    int file_fd;
    ssize_t read_size;

    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return 1;
    }

    server.sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, ip_address, &server.sin6_addr) <= 0)
    {
        perror("inet_pton");
        return 1;
    }
    server.sin6_port = htons(port);

    printf("Connecting to %s:%d\n", ip_address, ntohs(server.sin6_port));

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect");
        return 1;
    }

    printf("Connected to the server!\n");

    file_fd = open(FILENAME, O_RDONLY);
    if (file_fd == -1)
    {
        perror("open");
        return 1;
    }

    while ((read_size = read(file_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(sock, buffer, read_size) < 0)
        {
            perror("write");
            return 1;
        }
    }

    close(file_fd);
    close(sock);

    return 0;
}

int uds_dgram_client()
{
    int s, len;
    struct sockaddr_un remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect to server...\n");

    memset(&remote, 0, sizeof(remote));
    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected.\n");

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    size_t bytes_read;
    while ((bytes_read = fread(str, sizeof(char), sizeof(str), fp)) > 0) {
        if (send(s, str, bytes_read, 0) < 0) {
            perror("send");
            exit(1);
        }
    }

    fclose(fp);
    close(s);
    return 0;
}

int uds_stream_client()
{
      int s, len;
    struct sockaddr_un remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect to server...\n");

    memset(&remote, 0, sizeof(remote));
    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected.\n");

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    size_t bytes_read;
    while ((bytes_read = fread(str, sizeof(char), sizeof(str), fp)) > 0) {
        if (send(s, str, bytes_read, 0) < 0) {
            perror("send");
            exit(1);
        }
    }

    fclose(fp);
    close(s);
    return 0;
}

int mmap_client()
{
    int fd;
    void* addr;
    struct stat sb;

    // Open the file for reading
    if ((fd = open(FILENAME, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Get the size of the file
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    // Create a shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Resize the shared memory object to match the size of the file
    if (ftruncate(shm_fd, sb.st_size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory object into the address space of the calling process
    addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Copy the contents of the file into the shared memory object
    if (read(fd, addr, sb.st_size) != sb.st_size) {
        perror("read");
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
    return 0;
}

int pipe_client()
{
        int fd;
    int pipe_fd;
    char buf[BUFSIZ];
    ssize_t num_read;
    
    // Open the pipe for writing
    if ((pipe_fd = open(PIPE_NAME, O_WRONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the file for reading
    if ((fd = open(FILENAME, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read the file and write it to the pipe
    while ((num_read = read(fd, buf, BUFSIZ)) > 0) {
        if (write(pipe_fd, buf, num_read) != num_read) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file and the pipe
    close(fd);
    close(pipe_fd);
    return 0;
}

int client_options(int argv , char* argc[])
{
    
    if (argv < 7)
        perror("Usage: stnc -c IP PORT -p <type> <param>");

    // Set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    char ip_address[1024];
    int port;
    strcpy(ip_address, argc[2]);
    port = atoi(argc[3]);

    // Set up server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    // Send data over socket
    char type[1024], param[1024];
    strcpy(type, argc[5]);
    strcpy(param, argc[6]);

    char data[2048];
    sprintf(data, "%s %s", type, param);
    if (send(sockfd, data, strlen(data), 0) < 0)
    {
        perror("Error sending data");
        exit(1);
    }

    // Close socket
    close(sockfd);
    sleep(1);

    send_options_client(type, param, ip_address, port);
    return 0;
}

void send_options_client(char *type , char *param , char *ip_address , int port)
{
    switch (type[0])
    {
    case 'i':
        switch (param[0]) {
            case 't':
                ipv4_tcp_client(port, ip_address);
                break;
            case 'u':
                ipv4_udp_client(port, ip_address);
                break;
        }
        break;
    case 'm':
        mmap_client();
        break;
    case 'p':
        pipe_client();
        break;
    case 'u':
        switch (param[0])
        {
            case 'd':
                uds_dgram_client();
                break;
            case 's':
                uds_stream_client();
                break;
        }
        break;
    default:
        printf("Invalid option\n");
        break;
}
}
    //ipv4_tcp_client(atoi(argc[3]) , argc[2]);
    //ipv4_udp_client(atoi(argc[3]) , argc[2]);
    //ipv6_udp_client(atoi(argc[3]) , argc[2]);
    //ipv6_tcp_client(atoi(argc[3]) , argc[2]);
    //uds_dgram_client();
    //uds_stream_client();
    //mmap_client();
    //pipe_client();
