#include "socket.h"

void* serv(void* arg)
{
    int sockfd;
    char buffer[MAXLINE];
    char *frame = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    sockfd = s_udp();

    //erases the data in the bytes of the memory
    bzero(&servaddr,sizeof(servaddr));
    bzero(&cliaddr,sizeof(cliaddr));

    // Filling server information
    servaddr = s_addr(PORT);

    // Bind the socket with the server address
    s_bind(sockfd,servaddr);

    int len, n;

    len = sizeof(cliaddr);  //len is value/resuslt

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr,(socklen_t*)&len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)frame, strlen(frame),0, (const struct sockaddr *) &cliaddr,len);
    printf("Hello message sent.\n");

    pthread_exit(NULL);
}

void* cli(void* arg)
{
    int sockfd;
    char buffer[MAXLINE];
    char *frame = "Hello from client";
    struct sockaddr_in servaddr;

    
    // Creating socket file descriptor
    sockfd = s_udp();
    
    //erases the data in the bytes of the memory
    bzero(&servaddr,sizeof(servaddr));

    // Filling server information
    servaddr = s_ip_addr(ip1,PORT);

    int n, len;

    sendto(sockfd, (const char *)frame, strlen(frame),0, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    printf("Hello message sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,(socklen_t*)&len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);

    pthread_exit(NULL);
}



int main()
{
    state state_id0 = 0; //Waiting for beacon to sync
    int aux_beacon=0;
    state get_time=0;
    //***PCAP_Variables***//
    pcap_t *handler;
    char err_buf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr packet_header;
    const u_char *packet;
    //********************//

    //-----clocks-----//
    clock_t curr_clock,main_clock;

        while (1)
    {
        main_clock=clock();
        
        if (get_time == 1)
        {
            curr_clock=clock();
            main_clock=clock();
            get_time=0;
        }
        else if ( state_id0 == 0 && aux_beacon == 0 && get_time==0)
        {
            printf("Waiting for beacon\n");
            aux_beacon = pcap(device, handler, &packet_header, err_buf); // ainda vamos ter que mudar para a funçao que apenas 
            //procura o beacon - save time 
            
            printf("Aux beacon %d \n",aux_beacon);
            
            get_time = 1 ;
            state_id0 = 1;

            /*start downlink*/
        }

        else if (state_id0 == 1 && aux_beacon == 1 && get_time == 0 && ((float)(((main_clock - curr_clock) / 1000000.0F) * 1000) >= 50.0)) //sync, and start downlink
        {
            printf("State 1 \n");
            state_id0 = 2; 
            aux_beacon = 0;
            get_time=1;
            /*start tramit id1*/
        }
        else if (state_id0 == 2 && ((float)(((main_clock - curr_clock) / 1000000.0F) * 1000) >= 16.3)) //Downlink over
        {
            //Here we at 66.6
            printf("State 2\n");
            
            state_id0 = 3; //2->wait for slot to transmit
            
            
        }
        else if ( state_id0 == 3 ) //id1 is tramsiting
        {
            printf("State 3\n Id1 it's trasmiting");
            usleep(16300);
            state_id0 = 4;
        }
        else if ( state_id0 == 4 ) 
        {
            printf("State 4\n Id2 it's trasmiting");
            usleep(16300); // can be replaced for clock ---
            state_id0 = 0; //Finished transmiting and waiting for beacon to sync 
            //Start over 
            
        }
    }
}

    