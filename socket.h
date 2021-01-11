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
#include <pthread.h>
#include <pcap.h>

typedef struct sockaddr_in sa;
typedef struct ip_mreq m_req;

long timediff(clock_t t1, clock_t t2);
int s_udp();
sa s_addr(char *ip, int port);
void s_bind(int s, sa s_addr);
void s_reuse(int s);
void s_inet(int s, char *ip, m_req mreq);
void s_multicast(int s, m_req mreq);
#endif /* socket_h */