#include "socketslib.h"
#include <stdio.h>

int main()
{
    char buffer[1024];
    int server = socket_server_init("127.0.0.1", 3678);

    int s = socket_accept(server, NULL);

    socket_recv(s, buffer, sizeof(buffer), 0);

    printf("%s\n", buffer);

    socket_close(server);
}