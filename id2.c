#include "socket.h"

#define PORT1 8080
#define ip1 "127.0.0.1"
#define PORT0 8080
#define ip0 "127.0.0.1"

void *send_th(char *ip, int port, char *frame)
{
    int s = s_udp();
    sa dest;

    int rcv, len;
    char buffer[1024];

    //erases the data in the bytes of the memory
    bzero(&dest,sizeof(dest));

    //Filling destination (receiver) information
    dest = s_addr(ip1,PORT0);

    sendto(s, (const char*)frame, strlen(frame), 0, (const struct sockaddr*)&dest, sizeof(dest));

    rcv = recvfrom(s,(char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&dest, (socklen_t*)&len);
    //buffer[rcv] = '\0';
    //printf("message received: %s\n", buffer);
    close(s);
}

void *receive_th(char *ip, int port, char *frame)
{
    int s = s_udp();
    sa org, dest;

    char buffer[1024];
    int len, n;

    //erases the data in the bytes of the memory
    bzero(&org, sizeof(org));
    bzero(&dest, sizeof(dest));
    
    //Filling origin (receiver) information
    org = s_addr(ip,port);

    //Bind the socket with the origin (receiver) address
    s_bind(s,org);

    //dest
    len = sizeof(dest);

    n = recvfrom(s, (char*)buffer, 1024, MSG_WAITALL, (struct sockaddr*)&dest,(socklen_t*)&len);
    //buffer[n] = '\0';
    printf("message received: %s\n", buffer);

    sendto(s, (const char*)frame, strlen(frame),0,(struct sockaddr*)&dest, len);

}

int main()
{
    int state_id2 = 0; //Waiting for beacon to sync
    int aux_beacon, aux_50ms, offset, aux_16ms;
    
    
    while (1)
    {
        if ( state_id2 == 0 && aux_beacon == 1 ) //sync, and start downlink
        {
            state_id2 = 1; //1->start downlink, receive
            aux_50ms = timer(50);
            /*code*/
        }
        else if ( state_id2 == 1 && aux_50ms == 1 ) //Downlink over
        {
            state_id2 = 2; //2->wait for slot to transmit
            aux_50ms = 0;
            offset = timer(16+16);
        }
        else if ( state_id2 == 2 && offset == 1 ) //Uplink time to send socket
        {
            state_id2 = 3; //3->sending
            offset = 0;
            aux_16ms = timer(16);
            /*code*/
        }
        else if ( state_id2 == 3 && aux_16ms == 1 ) 
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

    sa sad_loc_f = s_addr(ip0,PORT0); //0
    sa sad_loc_b = s_addr(ip1,PORT1); //1
}