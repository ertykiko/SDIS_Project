#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>

#include "socket.h"

#define PORT1 1
#define ip1 "127.0.0.1"
#define PORT2 2
#define ip2 "127.0.0.1"

int main()
{
    int socket_f = s_udp_f(); //socket forward 1 
    int socket_b = s_udp_b(); //socket backward 2

    sa sad_loc_f = s_addr_f(ip1,PORT1); //1
    sa sad_loc_b = s_addr_f(ip2,PORT2); //2
    /*
    sad_loc_f.sin_family = AF_INET;
    sad_loc_f.sin_port = htons(PORT1);
    sad_loc_f.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 
    
    sad_loc_b.sin_family = AF_INET;
    sad_loc_b.sin_port = htons(PORT2);
    sad_loc_b.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    

    if ( (inet_aton(ip1, &sad_loc_f.sin_addr)==0) || (inet_aton(ip2, &sad_loc_b.sin_addr)==0) )
    {
        printf("\nInvalid address/ Address not supported \n");
        //return -1;
    }
    */


}