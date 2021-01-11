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
#include <time.h>
#include <stdbool.h>

#define MAXLINE 1024
#define PORT0 8080
#define PORT1 8080
#define PORT2 8080
#define ip0 "127.0.0.1"
#define ip1 "127.0.0.1"
#define ip2 "127.0.0.1"

typedef struct sockaddr_in sa;
typedef struct ip_mreq m_req;

int timer(int t);
void RTD(int t_send, int t_recv);
int s_udp();
sa s_addr(int port);
sa s_ip_addr(char *ip, int port);
void s_bind(int s, sa s_addr);
void s_reuse(int s);
void s_inet(int s, char *ip, m_req mreq);
void s_multicast(int s, m_req mreq);
int capture_beacon();
#endif /* socket_h */