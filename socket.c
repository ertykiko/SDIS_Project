#include "socket.h"

int timer(int t) //return 1 when t ms
{
    clock_t t1, t2;
    t1 = clock();
    int i;
    float diff;

    for(i = 0; i < 100000000; i++)
    {
        int x = 90;

        t2 = clock();

        diff = ((float)(t2 - t1) / 1000000.0F ) * 1000;

        if (diff >= t)
        {
          //printf("%f\n",diff);
          return 1;
        }
    }
    return 0;
}

void RTD(int t_send, int t_recv)
{
    FILE *fptr;

    float rtd = ((float)(t_recv - t_send) / 1000000.0F ) * 1000;

    fptr = fopen("tempo.txt", "a");
    fprintf(fptr, "%f\n", rtd);
    fclose(fptr);
}

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

sa s_addr(int port)
{
    sa s;

    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    s.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 

    return s;
}

sa s_ip_addr(char *ip, int port)
{
    sa s;

    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    if ((inet_aton(ip, &s.sin_addr)==0))
    {
        printf("\nInvalid address/ Address not supported\n");
        exit(0);
    }
    return s;
}

void s_bind(int s, sa addr)
{
    int b = bind(s, (struct sockaddr*)&addr,sizeof(addr));
    if (b<0)
    {
        printf("\nbind failed\n");
        return;
    }
}

void s_reuse(int s)
{
    int optval = 1;
    int ss = setsockopt(s,SOL_SOCKET,SO_REUSEADDR, &optval, sizeof(optval));
    if (ss<0)
    {
        printf("\nCan't Reuse address/ports\n");
        return;
    }
}

void s_inet(int s, char *ip, m_req mreq)
{
    int i = inet_aton(ip, &mreq.imr_interface);
    if (i==0)
    {
        printf("\nAddress %s bad", ip);
        return;
    }
    
}

void s_multicast(int s, m_req mreq)
{
    int set = setsockopt(s,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
    if (set!=0)
    {
        printf("\nJoin Multicast failed\n");
        return;
    }
}

int capture_beacon();
