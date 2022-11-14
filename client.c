#include "socketslib.h"

int main()
{
    int client = socket_client_init("127.0.0.1", 3678);

    socket_send(client, "hello world", 13, 0);

    socket_close(client);
}