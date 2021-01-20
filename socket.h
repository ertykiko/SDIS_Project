#ifndef socket_h
#define socket_h

#include <stdio.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h> 
#include <arpa/inet.h>	//inet_addr
#include <stdlib.h>
#include <pthread.h>
#include <pcap.h>
#include <time.h>

#define mac_addr_ap "48:f8:db:f8:ae:20"  //Acess Point MAC adress 
#define device "en1" // network device name
#define MAXLINE 1024
#define PORT0 8080
#define PORT1 8081
#define PORT2 8082

#define ip0 "127.0.0.1"
#define ip1 "127.0.0.1"
#define ip2 "127.0.0.1"

typedef struct sockaddr_in sa;
typedef int state;
typedef struct ip_mreq m_req;
typedef struct time_data{
    clock_t t_send;
    clock_t t_recv;
} time_data;
typedef struct aux_server{
  int i;
}aux_server;

int timer(int t);
void RTD(long t_send, long t_recv);
int s_udp();
sa s_addr(int port);
sa s_ip_addr(char *ip, int port);
void s_bind(int s, sa s_addr);
void s_reuse(int s);
void s_inet(int s, char *ip, m_req mreq);
void s_multicast(int s, m_req mreq);
int pcap(pcap_t *handler, struct pcap_pkthdr *packet_header);
void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);

#endif /* socket_h */
