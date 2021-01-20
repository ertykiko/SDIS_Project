#include "socket.h"

int timer(int t) //return 1 when t ms
{
    clock_t t1, t2;
    t1 = clock();
    int i;
    float diff;

    for (i = 0; i < 100000000; i++)
    {
        int x = 90;

        t2 = clock();

        diff = ((float)(t2 - t1) / 1000000.0F) * 1000;

        if (diff >= t)
        {
            //printf("%f\n",diff);
            return 1;
        }
    }
    return 0;
}

void RTD(long t_send, long t_recv)
{
    float rtd = (float)((t_recv - t_send)/1000000.0F);

    if(rtd < 0){
        return;
    } else {

    FILE *fptr;
    fptr = fopen("tempo.csv", "a");
    fprintf(fptr, "%f\n", rtd);
    fclose(fptr);
    }
}

int s_udp()
{
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        printf("\nSocket creation error\n");
        return -1;
    }
    return s;
}

sa s_addr(int port)
{
    sa s;

    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    //s.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede
    s.sin_addr.s_addr = INADDR_ANY;
    return s;
}

sa s_ip_addr(char *ip, int port)
{
    sa s;

    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    if ((inet_aton(ip, &s.sin_addr) == 0))
    {
        printf("\nInvalid address/ Address not supported\n");
        exit(0);
    }
    return s;
}

void s_bind(int s, sa addr)
{
    int b = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (b < 0)
    {
        printf("\nbind failed\n");
        return;
    }
}

void s_reuse(int s)
{
    int optval = 1;
    int ss = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ss < 0)
    {
        printf("\nCan't Reuse address/ports\n");
        return;
    }
}

void s_inet(int s, char *ip, m_req mreq)
{
    int i = inet_aton(ip, &mreq.imr_interface);
    if (i == 0)
    {
        printf("\nAddress %s bad", ip);
        return;
    }
}

void s_multicast(int s, m_req mreq)
{
    int set = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (set != 0)
    {
        printf("\nJoin Multicast failed\n");
        return;
    }
}
void my_packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    /* Do something with the packet here. 
       The print_packet_info() function shows in the
       previous example could be used here. */

    //print_packet_info(packet, *header);
}

int pcap(pcap_t *handler, struct pcap_pkthdr *packet_header)
{
    const u_char *packet = pcap_next(handler, packet_header);
    
    if (packet == NULL)
    {
        printf("Error reading Packet \n");
        return 0;
    }
    else
    {
        //printf("Sucess\n");
        //print_packet_info(packet,*packet_header);
        printf("Packet comment : %s\n",packet_header->comment);
        return 1;
    }
    
}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header)
{
    int len = 0;
    //printf("Packet %ld s , %d us \n", packet_header.ts.tv_sec, packet_header.ts.tv_usec);
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
    while (len < packet_header.len)
    {
        printf("%02x ", *(packet++));
        if (!(++len % 16))
            printf("\n");
    }
    printf("\n");
}


