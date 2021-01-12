#include "socket.h"
#define device "en1"
/*

void *send_thread(){


}


void *receive_thread(){

}

*/



int capture_beaconn(pcap_t *handler, struct pcap_pkthdr packet_header){

    /* 
        The packet  data is not to be freed by the caller, and is not guaranteed to
        be  valid  after  the  next  call   to   pcap_next_ex(),   pcap_next(),
        pcap_loop(),  or pcap_dispatch(); if the code needs it to remain valid,
        it must make a copy of it
    */
    pcap_t *local_handler ;
    struct pcap_pkthdr *local_packet_header;
    const u_char *local_packet_data;
    printf("Adress of Local is %p\n",&local_packet_header);
    printf("Adress of Global is %p \n",&packet_header);
    local_handler = handler; //copy data
    local_packet_header = &packet_header;
    
    printf("Adress of Local is %p\n", &local_packet_header);

    return (int)pcap_next(handler,&packet_header);
    //pcap_next()
    
}



  
}

    int main()
{
    int state_id2 = 0; //Waiting for beacon to sync
    int aux_beacon=0, aux_50ms, offset, aux_16ms;



    //***PCAP_Variables***//
    pcap_t *handler;
    char err_buf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr packet_header;
    const u_char *packet;
    //********************//

    bool debugg = false;

        //set_pcap(device,&handler,packet_header,err_buf);
        //printf("\nYo\n\n");
        //packet = pcap_next(&handler,&packet_header);

        printf("\nHero\n\n");
        //print_packet_info(packet, packet_header);
        printf("After packet info\n");
        while (1)
        {
            if (state_id2 == 0 && aux_beacon == 0)
            {
                clock_t t0 = clock();
                printf(".......\n");
                aux_beacon = pcap(device, handler, &packet_header, err_buf);
                if(debugg == true){

                clock_t t1 = clock();
                printf("%f\n", (float)((t1 - t0) / 1000000.0F) * 1000);
                printf(".......\n");
                printf("state id 2 %d\n",state_id2);
                printf("aux_beacon %d\n",aux_beacon);

                }
            }
            else if (state_id2 == 0 && aux_beacon == 1) //sync, and start downlink
            {
                state_id2 = 1; //1->start downlink, receive
                printf("Here\n");
                aux_50ms = timer(50);
                aux_beacon=0;
                /*code*/
            }
            else if (state_id2 == 1 && aux_50ms == 1) //Downlink over
            {
                state_id2 = 2; //2->wait for slot to transmit
                aux_50ms = 0;
                offset = timer(16 + 16);
            }
            else if (state_id2 == 2 && offset == 1) //Uplink time to send socket
            {
                state_id2 = 3; //3->sending
                offset = 0;
                aux_16ms = timer(16);
                /*code*/
            }
            else if (state_id2 == 3 && aux_16ms == 1)
            {
                state_id2 = 0; //Finished transmiting and waiting for beacon to sync
                aux_16ms = 0;
            }
    }
    //Downlink
    //receive beacon

    //Uplink
    //id2 -> start transmitting 50 + 16.6 + 16.6 ms

    int socket_f = s_udp(); //socket forward 0
    int socket_b = s_udp(); //socket backward 1

    sa sad_loc_f = s_addr(PORT0); //0
    sa sad_loc_b = s_addr(PORT1); //1

    return 0;
    }