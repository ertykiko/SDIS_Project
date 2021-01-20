#include "socket.h"
void avg_beacon_time(long *loop_number, int *avg_beacon_ms, long cur_beacon_s, long cur_beacon_us, long *last_beacon_s, long *last_beacon_us)
{
    FILE *fptr;
    fptr = fopen("intervalos.csv", "a");
    
    

    long aux_s;
    long aux_us;
    int auxi;

        if (*loop_number == 0)
    {
        //printf("First Beacon received, cannot calculate interval \n");
        fprintf(fptr, "Loop,Interval,Average\n");
        fprintf(fptr, "0,0,0\n" );
    }
    else
    {
        //aux_s = (*last_beacon_s - cur_beacon_s);
        aux_s = (cur_beacon_s - *last_beacon_s);
        aux_us = (cur_beacon_us - *last_beacon_us);
        //aux_us = (*last_beacon_us - cur_beacon_usec);
        auxi = (int)(aux_s * 1000 + aux_us / 1000);
        //printf("Beacon Interval %d \n :",auxi);
        if (*loop_number == 2)
        {
            *avg_beacon_ms = auxi;
            //printf("Average is the same as the interval \n");
        }
        if (*loop_number >= 3)
        {
            *avg_beacon_ms = *avg_beacon_ms + (auxi - *avg_beacon_ms) / (*loop_number);
            //printf("Average is : %d \n",*avg_beacon_ms);
        }
        fprintf(fptr, "%ld,%d,%d\n", *loop_number, auxi, *avg_beacon_ms);
    }

    

    *last_beacon_s = cur_beacon_s;
    *last_beacon_us = cur_beacon_us;
    fclose(fptr);
}
void *serv(void *arg)
{

    int sockfd;
    char buffer[MAXLINE];
    char *frame = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
    aux_server *aux = (aux_server *)arg;
    int len, n;

    if (aux->i == 0)
    {
        // Creating socket file descriptor
        sockfd = s_udp();
        //erases the data in the bytes of the memory
        bzero(&servaddr, sizeof(servaddr));
        bzero(&cliaddr, sizeof(cliaddr));

        // Filling server information
        servaddr = s_addr(PORT1);

        // Bind the socket with the server address
        s_bind(sockfd, servaddr);
        aux->i = 1;
    }

    while (1)
    {
        len = sizeof(cliaddr); //len is value/resuslt

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
        buffer[n] = '\0';
        if (n >= 0)
        {
            printf("Received : %s\n", buffer);
        }
        // sendto(sockfd, (const char *)frame, strlen(frame), 0, (const struct sockaddr *)&cliaddr, len);
        // printf("Hello message sent.\n");
    }

    pthread_exit(NULL);
}

void *cli(void *arg)
{
    int sockfd;
    char buffer[MAXLINE];
    char *frame = "Hello from client";
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    sockfd = s_udp();
    // s_reuse(sockfd);
    //erases the data in the bytes of the memory
    bzero(&servaddr, sizeof(servaddr));

    // Filling server information
    servaddr = s_ip_addr(ip1, PORT1);

    int n, len;

    sendto(sockfd, (const char *)frame, strlen(frame), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("cli -> serv\n");

    //****Recebe do Server****//
    // n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, (socklen_t *)&len);
    // buffer[n] = '\0';
    // printf("Server : %s\n", buffer);

    close(sockfd);

    pthread_exit(NULL);
}

int main()
{
    int firstpass = 1;
    //*****Setting up connection to wireless card ***//
    //***PCAP_Variables***//
    pcap_t *handler;
    struct pcap_pkthdr packet_header; //  && wlan type mgt subtype beacon src net 192.168.1.2 &&  && src net 192.168.1.0/24
    bpf_u_int32 mask;
    bpf_u_int32 ip;
    struct bpf_program fp;

    char error_buff[PCAP_ERRBUF_SIZE];              //Can be freed afer first pass ?
    //char filter[] = "wlan type mgt subtype beacon && ether host 48:f8:db:f8:ae:20"; //Can be freed after ? && src net 94.60.138.247
    //strcat(filter,mac_addr_ap);
    char *filter = malloc(sizeof(char)*61);
    strcpy(filter,"wlan type mgt subtype beacon && ether host ");
    strcat(filter,mac_addr_ap);

    bool debug = false;
    //********************//s
    if (firstpass == 1)
    {

        /* Get IP and Subnet-mask associated with capture device */
        if (pcap_lookupnet(device, &ip, &mask, error_buff) == -1)
        {
            fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device, error_buff);
            ip = 0;
            mask = 0;
        }

        /* print capture info */
        if (debug == true)
        {

            printf("Device: %s\n", device);
            printf("Filter expression: %s\n", filter);
        }

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
            fprintf(stderr, "Couldn't parse filter %s: %s\n", filter, pcap_geterr(handler));
            exit(EXIT_FAILURE);
        }

        /* apply the compiled filter */
        if (pcap_setfilter(handler, &fp) == -1)
        {
            fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handler));
            exit(EXIT_FAILURE);
        }
    }
    //*************///
    //states
    state st = 0; //Waiting for beacon to sync
    state get_time = 0;
    int aux_beacon = 0;
    //-----clocks-----//
    clock_t curr_clock, main_clock;
    //Relogios do quinaz muito bonitos
    clock_t send = 0, recv = 0;

    //Threads
    pthread_t pt_s;
    pthread_t pt_c;
    //bind server aux
    aux_server aux_s;
    aux_s.i = 0;

    int uno = 0;
    firstpass = 0;

    long loopnumber=0;
    int avg_beacon_ms=0;
    long last_beacon_us=0,last_beacon_s=0 ;

    
    while (1)
    {
        main_clock=clock();
        if(pcap(handler,&packet_header)){

        avg_beacon_time(&loopnumber,&avg_beacon_ms,packet_header.ts.tv_sec,packet_header.ts.tv_usec,&last_beacon_s,&last_beacon_us);
        loopnumber++;
        }
}
}

