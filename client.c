#include "easy_tcp_tls.h"
#include <stdio.h>

int main()
{
    SocketHandler client_handler;
    char buffer[100] = "hello world\n";

    socket_start();  // This is for Windows compatibility, it do nothing on Linux.

    socket_ssl_client_init(&client_handler, "127.0.0.1", 3678, NULL);

    socket_send(&client_handler, buffer, 15, 0);

    Sleep(1000);

    socket_close(&client_handler);

    socket_cleanup();  // again, for Windows compatibility
}