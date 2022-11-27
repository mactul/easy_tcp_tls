#include "easy_tcp_tls.h"
#include <stdio.h>

//openssl req -x509 -newkey rsa:4096 -nodes -out ./cert.pem -keyout ./key.pem -days 365

int main()
{
    char buffer[1024];
    SocketHandler server_handler;
    SocketHandler client_handler;

    socket_start();  // This is for Windows compatibility, it do nothing on Linux.
    
    printf("%d\n", socket_ssl_server_init(&server_handler, "127.0.0.1", 3678, 1, "cert.pem", "key.pem"));
    
    printf("%d\n", socket_accept(&client_handler, &server_handler, NULL));

    socket_recv(&client_handler, buffer, sizeof(buffer), 0);

    printf("%s\n", buffer);

    socket_close(&server_handler);
    socket_close(&client_handler);

    socket_cleanup();  // again, for Windows compatibility
}