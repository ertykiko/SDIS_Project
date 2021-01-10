#include "socket.h"

int sk_tcp()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    return sock;
}

int sk_udp()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    return sock;
}

struct sockaddr_in s_addr(char *ip, int port)
{
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(0);
    }
    return serv_addr;
}

struct sockaddr_in s_addr_fd(int port)
{
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    return serv_addr;
}

void s_connect(int sock, struct sockaddr_in serv_addr)
{
    int aux = 0;
    aux = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (aux < 0)
    {
        printf("Connection Failed \n");
        return;
    }
}

void s_bind(int s, struct sockaddr_in addr)
{
    int b = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (b < 0)
    {
        printf("bind failed");
        return;
    }
}

void s_listen(int s, int c)
{
    int l = listen(s, c);
    if (l < 0)
    {
        printf("listen error\n");
        return;
    }
}

int s_accept(int s, struct sockaddr_in addr)
{
    int addr_l = sizeof(addr);
    int a = accept(s, (struct sockaddr *)&addr, (socklen_t *)&addr_l);
    if (a < 0)
    {
        printf("accept error\n");
        return -1;
    }
    return a;
}