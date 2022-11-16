#include "socketslib.h"

int main()
{
    SocketHandler client_handler;
    char buffer[1024];
    char headers[] = "GET / HTTP/1.1\r\n"
        "Host: cdd-cloud.ml\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:106.0) Gecko/20100101 Firefox/106.0\r\n"
        "Connection: keep-alive\r\n\r\n";
    
    socket_ssl_client_init(&client_handler, "185.163.124.98", 443, "cdd-cloud.ml");

    socket_send(&client_handler, headers, strlen(headers)+1, 0);

    socket_recv(&client_handler, buffer, sizeof(buffer), 0);

    printf("%s\n", buffer);

    socket_close(&client_handler);
}