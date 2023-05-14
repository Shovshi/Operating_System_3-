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

#define FILENAME "received_file.bin"
#define BUFFER_SIZE 1024
#define SOCK_PATH "echo_socket"
#define SHM_NAME "/my_shm"
#define PAGE_SIZE 4096
#define PIPE_NAME "/tmp/my_pipe"

unsigned int checksum(char *filename);
int server_options(int argv, char *argc[]);
void send_options_server(char *type , char *param , int port , int qFlag);
int ipv4_tcp_server(int port , int qFlag);
int ipv4_udp_server(int port , int qFlag);
int mmap_server(int qFlag);
int pipe_server(int qFlag);
int uds_dgram_server(int qFlag);
int uds_stream_server(int qFlag);
int ipv6_tcp_server(int port , int qFlag);
int ipv6_udp_server(int port , int qFlag);


