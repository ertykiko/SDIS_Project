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
    int socket_f = socket(PF_INET, SOCK_DGRAM,0); //socket forward 0 
    int socket_b = socket(PF_INET, SOCK_DGRAM,0); //socket backward 1

    if ( (socket_f<0) || (socket_b<0))
    {
        printf("\n Socket creation error \n");
        //return -1;
    }
    
    sa sad_loc_f; //0
    sa sad_loc_b; //1

    sad_loc_f.sin_family = AF_INET;
    sad_loc_f.sin_port = htons(PORT0);
    sad_loc_f.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 

    sad_loc_b.sin_family = AF_INET;
    sad_loc_b.sin_port = htons(PORT1);
    sad_loc_b.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if ( (inet_aton(ip0, &sad_loc_f.sin_addr)==0) || (inet_aton(ip1, &sad_loc_b.sin_addr)==0) )
    {
        printf("\nInvalid address/ Address not supported \n");
        //return -1;
    }
}