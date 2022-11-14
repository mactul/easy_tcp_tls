#include "socketslib.h"

int socket_client_init(const char* server_ip, uint16_t server_port)
{
    #ifdef IS_WINDOWS
        WSADATA WSAData;
        WSAStartup(MAKEWORD(2,0), &WSAData);
    #endif

    struct sockaddr_in my_addr;
    int client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
        return SOCKET_ATTRIBUTION_ERROR;
    
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(server_port);
    
    // This ip address is the server ip address
    my_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    if (connect(client, (struct sockaddr*) &my_addr, sizeof my_addr) != 0)
        return CONNEXION_REFUSED;
    
    return client;
}

int socket_server_init(const char* server_ip, uint16_t server_port)
{
    #ifdef IS_WINDOWS
        WSADATA WSAData;
        WSAStartup(MAKEWORD(2,0), &WSAData);
    #endif
    
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval tv;

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    
    my_addr.sin_addr.s_addr = inet_addr(server_ip);
    my_addr.sin_port = htons(server_port);

    tv.tv_sec = 60;
    tv.tv_usec = 0;
    setsockopt(server, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
 
    if (bind(server, (struct sockaddr*) &my_addr, sizeof(my_addr)) != 0)
        return UNABLE_TO_BIND;
         
    if (listen(server, 1) != 0)
        return UNABLE_TO_LISTEN;
    
    return server;
}

int socket_accept(int server_fd, ClientData* pclient_data)
{
    struct sockaddr_in peer_addr;
    int addr_size;
    addr_size = sizeof(struct sockaddr_in);

    int s = accept(server_fd, (struct sockaddr*) &peer_addr, &addr_size);

    if(s > 0 && pclient_data != NULL)
    {
        inet_ntop(AF_INET, &(peer_addr.sin_addr), pclient_data->ip, INET_ADDRSTRLEN);
        pclient_data->port = ntohs(peer_addr.sin_port);
    }

    return s;
}

void socket_close(int fd)
{
    #ifdef IS_WINDOWS
        closesocket(fd);
        WSACleanup();
    #else
        close(fd);
    #endif
}