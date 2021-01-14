#include "socket.h"

// void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header)
// {
//     int len=0;
//     printf("Packet %ld , %d",packet_header.ts.tv_sec,packet_header.ts.tv_usec);
//     printf("Packet capture length: %d\n", packet_header.caplen);
//     printf("Packet total length %d\n", packet_header.len);
//     while (len < packet_header.len)
//     {
//         printf("%02x ", *(packet++));
//         if (!(++len % 16))
//             printf("\n");
//     }
//     printf("\n");
// }

// void my_packet_handler(u_char *args,const struct pcap_pkthdr *header,const u_char *packet)
// {
//     /* Do something with the packet here. 
//        The print_packet_info() function shows in the
//        previous example could be used here. */
//     //exit(EXIT_FAILURE);
// }

int main()
{
    pcap_t *handle;
    char error_buff[PCAP_ERRBUF_SIZE];
    char filter[]="wlan type mgt subtype beacon";
    char *dev = "en0";
    bpf_u_int32 mask;
    bpf_u_int32 ip;
    struct bpf_program fp;
    int num_packets=1;
    int timeout = 100;
    const u_char *packet;
    struct pcap_pkthdr packet_header;

    /* Get IP and Subnet-mask associated with capture device */
    if (pcap_lookupnet(dev, &ip, &mask, error_buff) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, error_buff);
        ip = 0;
        mask = 0;
    }

    /* print capture info */
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

   //Replace
    //handle = pcap_open_live(dev, BUFSIZ, 1, 1000, error_buff);

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

    
    pcap_set_snaplen(handle, 2304); // Set the snapshot length to 2304 MTU - 802.11
    pcap_set_promisc(handle, 0);    // Turn promiscuous mode off
    pcap_set_timeout(handle, 256);  // Set the timeout to 256 milliseconds

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
        fprintf(stderr, "Couldn't parse filter %s: %s\n",filter, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }
    
    /* now we can set our callback function */
    clock_t t0 = clock();
    printf(".......\n");
    packet = pcap_next(handle,&packet_header);
    //pcap_loop(handle,1,my_packet_handler,NULL);
    clock_t t1 = clock();
    printf("%f\n", (float)((t1 - t0) / 1000000.0F) *1000);
    printf(".......\n");
    print_packet_info(packet,packet_header);
    //printf("%d\n",debbug);

    //rtd = ((float)(t1- t_send) / 1000000.0F) * 1000

    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);

    printf("\nCapture complete.\n");

    return 0;

}