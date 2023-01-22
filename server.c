#include <stdint.h>  // this header have to be included before "easy_tcp_tls.h"
#include <stdio.h>
#include "easy_tcp_tls.h"

// generate key.pem and cert.pem with this shell command
// openssl req -x509 -newkey rsa:4096 -nodes -out ./cert.pem -keyout ./key.pem -days 365

int main()
{
    char buffer[1024];
    SocketHandler* server_handler;
    SocketHandler* client_handler;
    ClientData infos;

    socket_start();  // This is for Windows compatibility, it do nothing on Linux.
    
    server_handler = socket_ssl_server_init("127.0.0.1", 3678, 1, "cert.pem", "key.pem");
    
    if(server_handler == NULL)
    {
        socket_print_last_error();
        return 1;
    }

    client_handler = socket_accept(server_handler, &infos);  // We can also use NULL instead of &infos if we don't care about client infos
    if(client_handler == NULL)
    {
        socket_print_last_error();
        return 1;
    }

    printf("New client connected at:\n\tip: %s\n\tport: %d\n\n", infos.ip, infos.port);

    socket_recv(client_handler, buffer, sizeof(buffer), 0);

    printf("Message received: \"%s\"\n", buffer);

    socket_close(&client_handler);  // this is very important because it will free all structures
    socket_close(&server_handler);  // obviously, same comment

    socket_cleanup();  // again, for Windows compatibility
}