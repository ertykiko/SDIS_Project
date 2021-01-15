#include "socket.h"
#define device "wlp3s0"

int main(){

    // typedef enum state { get_time, state_id0} state;

    int state_id2 = 0; //Waiting for beacon to sync
    int state_id0 = 0;
    int get_time=0;
    int aux_beacon=0, aux_50ms, offset, aux_16ms;

    //***PCAP_Variables***//
    pcap_t *handler;
    struct pcap_pkthdr packet_header;
    char err_buf[PCAP_ERRBUF_SIZE];
    //struct pcap_pkthdr packet_header;
    const u_char *packet;
    //********************//

    bool debugg = false;


    char error_buff[PCAP_ERRBUF_SIZE];
    char filter[]="wlan type mgt subtype beacon";
    bpf_u_int32 mask;
    bpf_u_int32 ip;
    struct bpf_program fp;
    int num_packets=1;
    int timeout = 100;

    /* Get IP and Subnet-mask associated with capture device */
    if (pcap_lookupnet(device, &ip, &mask, error_buff) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device, error_buff);
        ip = 0;
        mask = 0;
    }

    /* print capture info */
    if(debugg==true)
    printf("Device: %s\n", device);
    printf("Number of packets: %d\n", num_packets);
    printf("Filter expression: %s\n", filter);

    /* open capture device */
    /*handler = pcap_open_live(dev, 3000, 1, timeout, error_buff);
      if (handler == NULL)
      {
      fprintf(stderr, "Couldn't open device %s: %s\n", dev, error_buff);
      exit(EXIT_FAILURE);
      }
      */

    //Replace
    //handler = pcap_open_live(dev, BUFSIZ, 1, 1000, error_buff);

    //create handler

    handler = pcap_create(device, error_buff);
    if (handler == NULL)
    {
        printf("Error create\n");
    }
    // Set device to monitor mode

    if (pcap_set_rfmon(handler, 1) != 0)
    {
        printf("Error while setting %s to monitor mode \n", device);
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
            fprintf(stderr, "%s is an ethernet device !\n", device);
        else
        {
            fprintf(stderr, "%s is not a wlan packet !\n", device);
        }

        exit(EXIT_FAILURE);
    }
    /* compile the filter expression */
    if (pcap_compile(handler, &fp, filter, 1, PCAP_NETMASK_UNKNOWN) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",filter, pcap_geterr(handler));
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handler, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handler));
        exit(EXIT_FAILURE);
    }

    //-----clocks-----//
    clock_t curr_clock,main_clock;

        while (1)
    {
        main_clock=clock;
        
        if (get_time == 1)
        {
            curr_clock=clock();
            main_clock=clock;
            get_time=0;
        }
        else if ( state_id0 == 0 && aux_beacon == 0 && get_time==0)
        {
            printf("Waiting for beacon\n");
            aux_beacon = pcap(device, handler, &packet_header, err_buf);
            
            printf("Aux beacon %d \n",aux_beacon);
            aux_beacon = 0;
            get_time = 1 ;
        }

    }

}

//     if (state_id2 == 0 && aux_beacon == 0)
//     {
//         clock_t t0 = clock();
//         printf(".......\n");
//         aux_beacon = pcap(device, handler, &packet_header, err_buf);
//         if (debugg == true)
//         {

//             clock_t t1 = clock();
//             printf("%f\n", (float)((t1 - t0) / 1000000.0F) * 1000);
//             printf(".......\n");
//             printf("state id 2 %d\n", state_id2);
//             printf("aux_beacon %d\n", aux_beacon);
//         }
//     }
//     else if (state_id2 == 0 && aux_beacon == 1) //sync, and start downlink
//     {
//         state_id2 = 1; //1->start downlink, receive
//         printf("Here\n");
//         aux_50ms = timer(50);
//         aux_beacon = 0;
//         /*code*/
//     }
//     else if (state_id2 == 1 && aux_50ms == 1) //Downlink over
//     {
//         state_id2 = 2; //2->wait for slot to transmit
//         aux_50ms = 0;
//         offset = timer(16 + 16);
//     }
//     else if (state_id2 == 2 && offset == 1) //Uplink time to send socket
//     {
//         state_id2 = 3; //3->sending
//         offset = 0;
//         aux_16ms = timer(16);
//         /*code*/
//     }
//     else if (state_id2 == 3 && aux_16ms == 1)
//     {
//         state_id2 = 0; //Finished transmiting and waiting for beacon to sync
//         aux_16ms = 0;
//     }
// }
// //Downlink
// //receive beacon

// //Uplink
// //id2 -> start transmitting 50 + 16.6 + 16.6 ms

// int socket_f = s_udp(); //socket forward 0
// int socket_b = s_udp(); //socket backward 1

// sa sad_loc_f = s_addr(PORT0); //0
// sa sad_loc_b = s_addr(PORT1); //1

// return 0;