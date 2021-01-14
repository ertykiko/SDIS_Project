#include "socket.h"
#define device "wlp3s0"

/*void set_pcap(char *dev, pcap_t *handler, char *err_buf)*/
/*{*/
    /*pcap_t *handler;*/
    /*char error_buff[PCAP_ERRBUF_SIZE];*/
    /*char filter[]="wlan type mgt subtype beacon";*/
    /*bpf_u_int32 mask;*/
    /*bpf_u_int32 ip;*/
    /*struct bpf_program fp;*/
    /*int num_packets=1;*/
    /*int timeout = 100;*/
    /*const u_char *packet;*/
    /*struct pcap_pkthdr packet_header;*/

    /*[> Get IP and Subnet-mask associated with capture device <]*/
    /*if (pcap_lookupnet(dev, &ip, &mask, error_buff) == -1)*/
    /*{*/
        /*fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, error_buff);*/
        /*ip = 0;*/
        /*mask = 0;*/
    /*}*/

    /*[> print capture info <]*/
    /*printf("Device: %s\n", dev);*/
    /*printf("Number of packets: %d\n", num_packets);*/
    /*printf("Filter expression: %s\n", filter);*/

    /*[> open capture device <]*/
    /**handler = pcap_open_live(dev, 3000, 1, timeout, error_buff);*/
    /*if (handler == NULL)*/
    /*{*/
        /*fprintf(stderr, "Couldn't open device %s: %s\n", dev, error_buff);*/
        /*exit(EXIT_FAILURE);*/
    /*}*/
    /**/

   /*//Replace*/
    /*//handler = pcap_open_live(dev, BUFSIZ, 1, 1000, error_buff);*/

    /*//create handler*/
    
    /*handler = pcap_create(dev, error_buff);*/
    /*if (handler == NULL)*/
    /*{*/
        /*printf("Error create\n");*/
    /*}*/
    /*// Set device to monitor mode*/

    /*if (pcap_set_rfmon(handler, 1) != 0)*/
    /*{*/
        /*printf("Error while setting %s to monitor mode \n", dev);*/
        /*exit(EXIT_FAILURE);*/
    /*}*/

    
    /*pcap_set_snaplen(handler, 2304); // Set the snapshot length to 2304 MTU - 802.11*/
    /*pcap_set_promisc(handler, 0);    // Turn promiscuous mode off*/
    /*pcap_set_timeout(handler, 256);  // Set the timeout to 256 milliseconds*/

    /*//activate device*/
    /*if (pcap_activate(handler) == PCAP_ERROR_ACTIVATED)*/
    /*{*/
        /*printf("Already atcivated \n");*/
    /*}*/
    
    /*//DLT_EN10MB - Ethernet*/
    /*//DLT_IEEE802_11_RADIO - Wifi*/

    /*if (pcap_datalink(handler) != DLT_IEEE802_11_RADIO)*/
    /*{*/
        /*if (pcap_datalink(handler) == DLT_EN10MB)*/
            /*fprintf(stderr, "%s is an ethernet device !\n", dev);*/
        /*else*/
        /*{*/
            /*fprintf(stderr, "%s is not a wlan packet !\n", dev);*/
        /*}*/

        /*exit(EXIT_FAILURE);*/
    /*}*/
    /*[> compile the filter expression <]*/
    /*if (pcap_compile(handler, &fp, filter, 1, PCAP_NETMASK_UNKNOWN) == -1)*/
    /*{*/
        /*fprintf(stderr, "Couldn't parse filter %s: %s\n",filter, pcap_geterr(handler));*/
        /*exit(EXIT_FAILURE);*/
    /*}*/

    /*[> apply the compiled filter <]*/
    /*if (pcap_setfilter(handler, &fp) == -1)*/
    /*{*/
        /*fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handler));*/
        /*exit(EXIT_FAILURE);*/
    /*}*/
 /*}*/

    int capture_beaconn(pcap_t * handler)
    {

        /* 
        The packet  data is not to be freed by the caller, and is not guaranteed to
        be  valid  after  the  next  call   to   pcap_next_ex(),   pcap_next(),
        pcap_loop(),  or pcap_dispatch(); if the code needs it to remain valid,
        it must make a copy of it
    */
        // pcap_t *local_handler;
        // struct pcap_pkthdr *local_packet_header;
        struct pcap_pkthdr *packet_header;
        const u_char *local_packet_data;
        // printf("Adress of Local is %p\n", &local_packet_header);
        // printf("Adress of Global is %p \n", &packet_header);
        // local_handler = handler; //copy data
        // local_packet_header = packet_header;

        // printf("Adress of Local is %p\n", &local_packet_header);
        printf("Adress of Global is %p \n", &handler);
        // return (int)pcap_next(handler, packet_header);
        printf("Here \n");
           return  pcap_next(handler, &packet_header);
        //pcap_next()
    }


    int main(){
    int state_id2 = 0; //Waiting for beacon to sync
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







        while (1)
        {
             
            if(state_id2==0 && aux_beacon==0){

                packet = pcap_next(handler,&packet_header);

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
