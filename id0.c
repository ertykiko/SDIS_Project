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



// int main()
// {
//     int state_id0 = 0; //Waiting for beacon to sync
//     int aux_beacon=0, aux_50ms, offset, aux_16ms;
    
//     //***PCAP_Variables***//
//     pcap_t *handler;
//     char err_buf[PCAP_ERRBUF_SIZE];
//     struct pcap_pkthdr packet_header;
//     const u_char *packet;
//     //********************//

//         while (1)
//     {
//         if ( state_id0 == 0 && aux_beacon == 0)
//         {
//             printf("State 1\n");
//             //aux_beacon = pcap(device, handler, &packet_header, err_buf);
//             printf("Aux beacon %d \n",aux_beacon);
//         }
//         else if ( state_id0 == 0 && aux_beacon == 1 ) //sync, and start downlink
//         {
//             printf("State 2\n");
//             state_id0 = 1; //1->start downlink, receive
//             aux_beacon = 0;
//             aux_50ms = timer(50);
//             /*code*/
//         }
//         else if ( state_id0 == 1 && aux_50ms == 1 ) //Downlink over
//         {
//             printf("State 3\n");
//             state_id0 = 2; //2->wait for slot to transmit
//             aux_50ms = 0;
//             //offset = timer(0);
//             offset = 1;
//         }
//         else if ( state_id0 == 2 && offset == 1 ) //Uplink time to send socket
//         {
//             printf("State 4\n");
//             state_id0 = 3; //3->sending
//             offset = 0;
//             aux_16ms = timer(16);
//             /*code*/
//         }
//         else if ( state_id0 == 3 && aux_16ms == 1 ) 
//         {
//             printf("State 5\n");
//             state_id0 = 0; //Finished transmiting and waiting for beacon to sync 
//             aux_16ms = 0;  
            
//         }
//     }
// }

//teste main
// int main()
// {
//     pthread_t pt_s;
//     pthread_t pt_c;

//     pthread_create(&pt_s,NULL,serv,NULL);
//     pthread_join(pt_s,NULL);

//     pthread_create(&pt_c,NULL,cli,NULL);
//     pthread_join(pt_c,NULL);
    
//     return 0;
// }