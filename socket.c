#include "socket.h"

int s_udp()
{
    int s = socket(PF_INET, SOCK_DGRAM,0);
    if ( s<0 )
    {
        printf("\n Front Socket creation error\n");
        return -1;
    }
    return s;
}



sa s_addr(char *ip, int port)
{
    sa s;

    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    s.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 

    if ((inet_aton(ip, &s.sin_addr)==0))
    {
        printf("\nInvalid address/ Address not supported\n");
        exit(0);
    }

    return s;
}

void s_bind(int s, sa addr)
{
    int addr_len=sizeof(addr);
    int b = bind(s, (struct sockaddr*)&addr,(socklen_t*)&addr_len);
    if (b<0)
    {
        printf("\nbind failed\n");
        return;
    }
}