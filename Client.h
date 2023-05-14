
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


