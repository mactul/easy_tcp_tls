#include <stdint.h>  // this header have to be included before "easy_tcp_tls.h"
#include "easy_tcp_tls.h"

int main()
{
    SocketHandler* client_handler;
    char buffer[100] = "hello world";

    socket_start();  // This is for Windows compatibility, it do nothing on Linux.

    client_handler = socket_ssl_client_init("127.0.0.1", 3678, NULL);

    if(client_handler == NULL)
    {
        socket_print_last_error();
        return 1;
    }

    socket_send(client_handler, buffer, 15, 0);

    socket_close(&client_handler);  // this is very important because it will free all structures

    socket_cleanup();  // again, for Windows compatibility
}