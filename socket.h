#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> //inet_addr
#include <stdlib.h>

typedef int sockfd;
typedef struct sockaddr_in socka;

int sk_tcp();
int sk_udp();
struct sockaddr_in s_addr(char *ip, int port);
struct sockaddr_in s_addr_fd(int port);
void s_connect(int s, struct sockaddr_in addr);
void s_bind(int s, struct sockaddr_in addr);
void s_listen(int s, int c);
int s_accept(int s, struct sockaddr_in addr);
