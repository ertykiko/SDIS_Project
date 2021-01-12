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
void my_packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    /* Do something with the packet here. 
       The print_packet_info() function shows in the
       previous example could be used here. */

    //print_packet_info(packet, *header);
    
}

int pcap(char *dev, pcap_t *handler, struct pcap_pkthdr *packet_header, char *err_buf)
{
    bpf_u_int32 mask;
    bpf_u_int32 ip;
    struct bpf_program fp;
    char filter[] = "wlan type mgt subtype beacon";
    //char *dev = "en1";

    /*Get Ip and Subnet Mask associated with capture device*/
    if (pcap_lookupnet(dev, &ip, &mask, err_buf) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, err_buf);
        ip = 0;
        mask = 0;
    }

    /*
    Create Handle with pre-defined parameters :
    Snaplen = 2304
    Promiscuous mode OFF (==0)
    Timeout = 256 ms 
    Monitor mode ON (==1)
    */
    handler = pcap_create(dev, err_buf);
    if (handler == NULL)
    {
        printf("Error create\n");
    }
    // Set device to monitor mode

    if (pcap_set_rfmon(handler, 1) != 0)
    {
        printf("Error while setting %s to monitor mode \n", dev);
        exit(EXIT_FAILURE);
    }

    pcap_set_snaplen(handler, 2304); // Set the snapshot length to 2304 MTU - 802.11
    pcap_set_promisc(handler, 0);    // Turn promiscuous mode off
    pcap_set_timeout(handler, 256);  // Set the timeout to 256 milliseconds

    //activate device
    if (pcap_activate(handler) == PCAP_ERROR_ACTIVATED)
    {
        printf("Already atcivated \n");
    }
    //DLT_EN10MB - Ethernet
    //DLT_IEEE802_11_RADIO - Wifi

    if (pcap_datalink(handler) != DLT_IEEE802_11_RADIO)
    {
        if (pcap_datalink(handler) == DLT_EN10MB)
            fprintf(stderr, "%s is an ethernet device !\n", dev);
        else
        {
            fprintf(stderr, "%s is not a wlan packet !\n", dev);
        }

        exit(EXIT_FAILURE);
    }
    /* compile the filter expression */
    if (pcap_compile(handler, &fp, filter, 1, PCAP_NETMASK_UNKNOWN) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter, pcap_geterr(handler));
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handler, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handler));
        exit(EXIT_FAILURE);
    }

    const u_char *packet;
    /*printf("Yo\n");

    //packet = pcap_next(handler, &packet_header);
    printf("Go\n");
    print_packet_info(packet, packet_header);
    printf("Herozo\n");*/

    if (pcap_next(handler, packet_header) == NULL)
    {
        printf("Error reading Packet \n");
        return 0;
    }
    else
    {
        //printf("Sucess\n");
        return 1;
    }

}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header)
{
    int len = 0;
    printf("Packet %ld , %d", packet_header.ts.tv_sec, packet_header.ts.tv_usec);
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