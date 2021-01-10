#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include "socket.h"

#define PORT1 1
#define ip1 "127.0.0.1"
#define PORT0 0
#define ip0 "127.0.0.1"

int main()
{
    int socket_f = s_udp_f(); //socket forward 0 
    int socket_b = s_udp_b(); //socket backward 1

    sa sad_loc_f = s_addr_f(ip0,PORT0); //0
    sa sad_loc_b = s_addr_f(ip1,PORT1); //1
}