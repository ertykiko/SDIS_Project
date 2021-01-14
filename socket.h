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

#define device "en1"
#define MAXLINE 1024
#define PORT 8080
#define ip0 "10.10.10.195"
#define ip1 "10.10.10.195"
#define ip2 "10.10.10.195"

typedef struct sockaddr_in sa;
typedef int state;
typedef struct ip_mreq m_req;
typedef struct time_data{
    clock_t t_send;
    clock_t t_recv;
} time_data;

int timer(int t);
void RTD(clock_t t_send, clock_t t_recv);
int s_udp();
sa s_addr(int port);
sa s_ip_addr(char *ip, int port);
void s_bind(int s, sa s_addr);
void s_reuse(int s);
void s_inet(int s, char *ip, m_req mreq);
void s_multicast(int s, m_req mreq);
//int pcap(char *dev, pcap_t *handler, struct pcap_pkthdr *packet_header, char *err_buf);


void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);

#endif /* socket_h */