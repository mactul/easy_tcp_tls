#include <stdint.h>
#ifdef __unix__
    #include <unistd.h>
    #include <netdb.h>
    #include <errno.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>

#elif defined(_WIN32) || defined(WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    #define IS_WINDOWS 1
    //#define INET_ADDRSTRLEN 16

#endif

#define socket_send(fd, buffer, n, flags) send(fd, buffer, n, flags)
#define socket_recv(fd, buffer, n, flags) recv(fd, buffer, n, flags);

enum ERROR_CODES {
    SOCKET_ATTRIBUTION_ERROR = -1,
    CONNEXION_REFUSED = -2,
    UNABLE_TO_BIND = -3,
    UNABLE_TO_LISTEN = -4
};

typedef struct client_data {
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
} ClientData;

int socket_client_init(const char* server_ip, uint16_t server_port);
int socket_server_init(const char* server_ip, uint16_t server_port);
int socket_accept(int server_fd, ClientData* pclient_data);
void socket_close(int fd);