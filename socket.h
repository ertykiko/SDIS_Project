#ifndef socket_h
#define socket_h


#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
//#include <inttypes.h>
#include <string.h> 
#include <arpa/inet.h>	//inet_addr
#include <stdlib.h>

typedef struct sockaddr_in sa;

int s_udp_f();
int s_udp_b();
sa s_addr_f(char *ip, int port);
sa s_addr_b(char *ip, int port);

#endif /* socket_h */