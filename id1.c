#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include "socket.h"

#define PORT0 0
#define ip0 "127.0.0.1"
#define PORT2 2
#define ip2 "127.0.0.1"

main()
{
    int socket_f = s_udp(); //socket forward 2 
    int socket_b = s_udp(); //socket backward 0

    sa sad_loc_f = s_addr(ip2,PORT2); //2
    sa sad_loc_b = s_addr(ip0,PORT0); //0
    
}