#include "socket.h"

void *serv(void *arg)
{

    int sockfd;
    char buffer[MAXLINE];
    char *frame = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
    aux_server *aux = (aux_server *)arg;
    int len, n;

    if ( aux->i == 0 )
    {
        // Creating socket file descriptor
        sockfd = s_udp();
        //erases the data in the bytes of the memory
        bzero(&servaddr, sizeof(servaddr));
        bzero(&cliaddr, sizeof(cliaddr));

        // Filling server information
        servaddr = s_addr(PORT0);

        // Bind the socket with the server address
        s_bind(sockfd, servaddr);
        aux->i = 1;
    }

    // while (1)
    // {
        len = sizeof(cliaddr); //len is value/resuslt

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
        buffer[n] = '\0';
        if ( n >= 0 )
        {
            printf("Received : %s on id0'\n", buffer);
        }
        // sendto(sockfd, (const char *)frame, strlen(frame), 0, (const struct sockaddr *)&cliaddr, len);
        // printf("Hello message sent.\n");
    // }

    pthread_exit(NULL);
}

void *cli(void *arg)
{
    int sockfd;
    char buffer[MAXLINE];
    char *frame = "Hello from client id0";
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

int main(int argc, char **argv)
{
    //Threads
    pthread_t pt_s;
    pthread_t pt_c;
    //states
    state wait = 0;
    state state_id0 = 2;
    //aux
    int beacon_aux;
    //bind server aux
    aux_server aux_s;
    aux_s.i = 0;
    //PCAP_Variables
    pcap_t *handler;
    struct pcap_pkthdr packet_header;
    //clock
    clock_t send = 0,recv = 0;
    // while (1)
    // {
    //     if ( wait == 0 && beacon_aux == 1) //recebeu o beacon pode transmitir
    //     {
    //         pthread_create(&pt_c,NULL,cli,NULL);
    //         wait = 1;
    //     }
    //     else if ( wait == 1 && ( pthread_join(pt_c,NULL) == 0 ) )
    //     {
    //         pthread_create(&pt_s,NULL,serv,NULL);
    //         wait = 2;
    //     }
    //     else if ( wait == 2 &&  ( pthread_join(pt_s,NULL) == 0 ) )
    //     {
    //         wait = 0;
    //     }
    // }
    while(1)
    {
        if( state_id0 ==0)
        {
           //start server
            pthread_create(&pt_s, NULL, serv, (void *)&aux_s);
            state_id0++;
        }
        else if ( state_id0 == 1 && !pthread_join(pt_s, NULL))
        {
            recv=clock();
            RTD(send,recv);
            state_id0++;
        }
        else if (state_id0 == 2 && pcap(handler,&packet_header))
        {
            pthread_create(&pt_c, NULL, cli, NULL);
            send = clock();
            state_id0=0;
        }
    }    
}
