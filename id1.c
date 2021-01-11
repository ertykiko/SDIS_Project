#include "socket.h"

#define PORT0 8080
#define ip0 "127.0.0.1"
#define PORT2 8080
#define ip2 "127.0.0.1"

int main()
{
    int state_id1 = 0; //Waiting for beacon to sync
    int aux_beacon, aux_50ms, offset, aux_16ms;
    
    
    while (1)
    {
        if ( state_id1 == 0 && aux_beacon == 1 ) //sync, and start downlink
        {
            state_id1 = 1; //1->start downlink, receive
            aux_50ms = timer(50);
            /*code*/
        }
        else if ( state_id1 == 1 && aux_50ms == 1 ) //Downlink over
        {
            state_id1 = 2; //2->wait for slot to transmit
            aux_50ms = 0;
            offset = timer(16);
        }
        else if ( state_id1 == 2 && offset == 1 ) //Uplink time to send socket
        {
            state_id1 = 3; //3->sending
            offset = 0;
            aux_16ms = timer(16);
            /*code*/
        }
        else if ( state_id1 == 3 && aux_16ms == 1 ) 
        {
            state_id1 = 0; //Finished transmiting and waiting for beacon to sync 
            aux_16ms = 0;  
        }
    }
    //Downlink 
    //receive beacon

    //Uplink
    //id1 -> start transmitting 50 + 16,6 ms 
    

    int socket_f = s_udp(); //socket forward 2 
    int socket_b = s_udp(); //socket backward 0

    sa sad_loc_f; //2
    sa sad_loc_b; //0
    
    m_req mreq;

    //reuse
    s_reuse(socket_f);

    //erases the data
    bzero(&sad_loc_f,sizeof(sad_loc_f));
    bzero(&sad_loc_f,sizeof(sad_loc_b));

    //addr
    sad_loc_f = s_addr(ip2,PORT2); //2
    sad_loc_b = s_addr(ip0,PORT0); //0

    //bind
    s_bind(socket_f,sad_loc_f);

    //inet_aton
    s_inet(socket_f,ip2,mreq);

    //multicast
    s_multicast(socket_f,mreq);

}   