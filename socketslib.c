#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

enum ERROR_CODES {
    SOCKET_ATTRIBUTION_ERROR = -1,
    CONNEXION_REFUSED = -2,
    UNABLE_TO_BIND = -3,
    UNABLE_TO_LISTEN = -4
};


int socket_client_init(const char* server_ip, uint16_t server_port)
{
    struct sockaddr_in my_addr;
    int client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
        return SOCKET_ATTRIBUTION_ERROR;
    
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(server_port);
    
    // This ip address is the server ip address
    my_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    socklen_t addr_size = sizeof my_addr;
    if (connect(client, (struct sockaddr*) &my_addr, sizeof my_addr) != 0)
        return CONNEXION_REFUSED;
    
    return client;
}
}
}
}

int socket_server_init(const char* server_ip, const char* server_port)
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval tv;

    struct sockaddr_in my_addr, peer_addr;
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

    socklen_t addr_size;
    addr_size = sizeof(struct sockaddr_in);

    while (1)
    {
        int acc = accept(server, (struct sockaddr*) &peer_addr, &addr_size);
    }
}


int socket_send(int fd, const void* buffer, size_t n, int flags)
{
    send(fd, buffer, n, flags);
}

void socket_close(int fd)
{
    close(fd);
}