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

#define FILENAME "large_file.bin"
#define BUFFER_SIZE 1024
#define SOCK_PATH "echo_socket"
#define SHM_NAME "/my_shm"
#define PAGE_SIZE 4096
#define PIPE_NAME "/tmp/my_pipe"

void create_large_file(); 
int client_options(int argv, char *argc[]);
void send_options_client(char *type , char *param ,char *ip_address , int port);
int ipv4_tcp_client(int port , char *ip_address);
int ipv4_udp_client(int port , char *ip_address);
int mmap_client();
int pipe_client();
int uds_dgram_client();
int uds_stream_client();
int ipv6_tcp_client(int port , char *ip_address);
int ipv6_udp_client(int port , char *ip_address);


