#include "socket.h"
#include "beacon.c"

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

    int rtd = ((float)(t_recv - t_send) / 1000000.0F ) * 1000;

    fptr = fopen("tempo.txt", "a");
    fprintf(fptr, "%d\n", rtd);
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

sa s_addr(char *ip, int port)
{
    sa s;

    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    s.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 

    /*
    if ((inet_aton(ip, &s.sin_addr)==0))
    {
        printf("\nInvalid address/ Address not supported\n");
        exit(0);
    }
    */
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
int my_packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    /* Do something with the packet here. 
       The print_packet_info() function shows in the
       previous example could be used here. */

    //print_packet_info(packet, *header);
    return 1;
}
int capture_beacon(char * dev,int timeout,bool debugg ){
    pcap_t *handle;
    char error_buff[PCAP_ERRBUF_SIZE];
    char filter[] = "wlan type mgt subtype beacon";
    bpf_u_int32 mask;
    bpf_u_int32 ip;
    struct bpf_program fp;
    int num_packets = 1;
    

    /* Get IP and Subnet-mask associated with capture device */
    if (pcap_lookupnet(dev, &ip, &mask, error_buff) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, error_buff);
        ip = 0;
        mask = 0;
    }

    /* print capture info */
    if(debugg==true)
    printf("Device: %s\n", dev);
    printf("Number of packets: %d\n", num_packets);
    printf("Filter expression: %s\n", filter);

    /* open capture device */
    /*handle = pcap_open_live(dev, 3000, 1, timeout, error_buff);
    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, error_buff);
        exit(EXIT_FAILURE);
    }
    */

    //Replace pcap_open_live()

    //create handle
    handle = pcap_create(dev, error_buff);
    if (handle == NULL)
    {
        printf("Error create\n");
    }
    // Set device to monitor mode

    if (pcap_set_rfmon(handle, 1) != 0)
    {
        printf("Error while setting %s to monitor mode \n", dev);
        exit(EXIT_FAILURE);
    }

    //Beacon Broadcast ?
    //pcap_set_promisc(handle, 1); /* Capture packets that are not yours */

    //activate device
    if (pcap_activate(handle) == PCAP_ERROR_ACTIVATED)
    {
        printf("Already atcivated \n");
    }

    //DLT_EN10MB - Ethernet
    //DLT_IEEE802_11_RADIO - Wifi

    if (pcap_datalink(handle) != DLT_IEEE802_11_RADIO)
    {
        if (pcap_datalink(handle) == DLT_EN10MB)
            fprintf(stderr, "%s is an ethernet device !\n", dev);
        else
        {
            fprintf(stderr, "%s is not a wlan packet !\n", dev);
        }

        exit(EXIT_FAILURE);
    }
    /* compile the filter expression */
    if (pcap_compile(handle, &fp, filter, 1, PCAP_NETMASK_UNKNOWN) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* now we can set our callback function */

    pcap_loop(handle, num_packets, my_packet_handler, NULL);

    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);
    if(debugg==true)
    printf("\nCapture complete.\n");
}

}