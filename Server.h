#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/un.h>
#include <sys/mman.h>   
#include <sys/stat.h>

#define FILENAME "received_file.txt"
#define BUFFER_SIZE 1024
#define SOCK_PATH "echo_socket"
#define SHM_NAME "/my_shm"
#define PAGE_SIZE 4096
#define PIPE_NAME "/tmp/my_pipe"

int server_options(int argv, char *argc[]);
void send_options_server(char *type , char *param , int port)
int ipv4_tcp_server(int port);
int ipv4_udp_server(int port);
int mmap_server();
int pipe_server();
int uds_dgram_server();
int uds_stream_server();
int ipv6_tcp_server(int port);
int ipv6_udp_server(int port);


