
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


