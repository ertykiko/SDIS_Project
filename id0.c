#include "socket.h"

#define PORT1 9930
#define ip1 "127.0.0.1"
#define PORT2 2
#define ip2 "127.0.0.1"


    int main(int argc, char **argv)
    {
        sockfd sfd;
        int n, i, pck_count;
        socklen_t len;
        long sleep_time, proc_time;
        struct timeval start, end, val1;

        // time_t ltime;

        /* 
	 * socketaddr_in is a structure containing an Internet socket address.
	 * It contains: an address family, a port number, an IP address
	 */
        sa socka;

        if (argc != 4)
        {
            printf("Usage: %s ipaddress numOfPackets sleeptime(microsec)\n", argv[0]);
            return -1;
        }

        // assigning the second argument as number of packets
        pck_count = atoi(argv[2]);
        sleep_time = atol(argv[3]);

        /* 
	 * Create a socket.
	 * AF_INET says that it will be an Internet socket.
	 * SOCK_DGRAM says that it will use datagram delivery instead of virtual circuits.
	 * IPPROTO_UDP says that it will use the UDP protocol
	 */
        if ((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
            error("socket");

        // Initialize saddr strucure, filling with binary zeros
        bzero(&socka, sizeof(socka));

        socka.sin_family = AF_INET;

        socka.sin_port = htons(PORT1); // htons() ensures that the byte order is correct (Host TO Network order/Short integer)
        //inet_pton(AF_INET, argv[1], &caddr.sin_addr);

        if (inet_aton(argv[1], &socka.sin_addr) == 0)
        {
            fprintf(stderr, "inet_aton() failed.\n");
            exit(1);
        }

        //printf("Client Running... ...\n");

        for (i = 0; i < pck_count; i++)
        {
            struct UDP pck;
            bzero(&pck, sizeof(pck));

            len = sizeof(socka);

            gettimeofday(&val1, NULL);

            usleep(sleep_time);

            //get wakeuptime ~ sendtime
            gettimeofday(&start, NULL);

            long sleep_ov = (start.tv_sec * 1000000 + start.tv_usec) - (val1.tv_sec * 1000000 + val1.tv_usec) - sleep_time;
            //		sleep_ov = sleep_ov / 1000;
            //		if(sleep_ov > 10) {
            printf("sleep overhead %ld micro s\n", sleep_ov);
            //		}
            //printf("");
            //printf("%lu ", (start.tv_sec * 1000000 + start.tv_usec) - (val1.tv_sec * 1000000 + val1.tv_usec));

            pck.packet_id = i;
            pck.send_time_sec = start.tv_sec;
            pck.send_time_usec = start.tv_usec;
            //printf(" %lu\n", pck.send_time);
            pck.sleep_time = sleep_time;

            /*
		 * Send sizeof(pck) bytes from pck to sfd, with no flags (0).
		 * The receiver is specified in caddr, which contains len byte. 
		 */
            // ltime = time(NULL);
            if (sendto(sfd, (const char *)&pck, sizeof(pck), 0, (struct sockaddr *)&socka, len) == -1)
                error("sendto()");

            //gettimeofday(&end, NULL);

            // uncomment following line to see information in console
            //printf("Sending packet %d from %s:%d, time: %ld.%ld\n", pck.pck_id, inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port), pck.send_time_sec, pck.send_time_usec);

            // calculating the processing time
            // proc_time = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
            //printf("Send Processing time: %ld\n", proc_time);
        }

        close(sfd);
        return 0;
    }
