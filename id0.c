#include "socket.h"

#define PORT1 8080
#define ip1 "127.0.0.1"
#define PORT2 8080
#define ip2 "127.0.0.1"

int main()
{
    int state_id0 = 0; //Waiting for beacon to sync
    int aux_beacon, aux_50ms, offset, aux_16ms;
    
    
    while (1)
    {
        if ( state_id0 == 0 && aux_beacon ) //sync, and start downlink
        {
            state_id0 = 1; //1->start downlink, receive
            /*code*/
        }
        else if ( state_id0 == 1 && aux_50ms ) //Downlink over
        {
        
            state_id0 = 2; //2->wait for slot to transmit
        }
        else if ( state_id0 == 2 && offset ) //Uplink time to send socket
        {
            state_id0 = 3; //3->sending
            /*code*/
        }
        else if ( state_id0 == 3 && aux_16ms ) 
        {
            state_id0 = 0; //Finished transmiting and waiting for beacon to sync   
        }
    }
    
    
    
    
    
    
    int socket_f = s_udp(); //socket forward 1 
    int socket_b = s_udp(); //socket backward 2

    sa sad_loc_f = s_addr(ip1,PORT1); //1
    sa sad_loc_b = s_addr(ip2,PORT2); //2
    /*
    sad_loc_f.sin_family = AF_INET;
    sad_loc_f.sin_port = htons(PORT1);
    sad_loc_f.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //INADDR_LOOPBACK: ip que desgina o computador local, usando o loopback device: as mensagens não circulam na rede 
    
    sad_loc_b.sin_family = AF_INET;
    sad_loc_b.sin_port = htons(PORT2);
    sad_loc_b.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    

    if ( (inet_aton(ip1, &sad_loc_f.sin_addr)==0) || (inet_aton(ip2, &sad_loc_b.sin_addr)==0) )
    {
        printf("\nInvalid address/ Address not supported \n");
        //return -1;
    }
    */


}