#include "socket.h"

int s_udp_f()
{
    int s = socket(PF_INET, SOCK_DGRAM,0);
    if ( s<0 )
    {
        printf("\n Front Socket creation error \n");
        return -1;
    }
    return s;
}

int s_udp_b()
{
    int s = socket(PF_INET, SOCK_DGRAM,0);
    if ( s<0 )
    {
        printf("\n Back Socket creation error \n");
        return -1;
    }
    return s;
}

sa s_addr_f(char *ip, int port)
{
    sa s_f;

    s_f.sin_family = AF_INET;
    s_f.sin_port = htons(port);
    s_f.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 

    if ((inet_aton(ip, &s_f.sin_addr)==0))
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(0);
    }

    return s_f;
}

sa s_addr_b(char *ip, int port)
{
    sa s_b;

    s_b.sin_family = AF_INET;
    s_b.sin_port = htons(port);
    s_b.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 
    
    if ((inet_aton(ip, &s_b.sin_addr)==0))
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(0);
    }
    
    return s_b;
}