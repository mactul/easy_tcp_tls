#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "easy_tcp_tls.h"

/*********************************************************/
/*                    /!\ WARNING /!\                    */
/*********************************************************/

/*  This is just an example of how to use this library.  */
/*   If you want to create HTTP/HTTPS requests easily,   */
/*    I recommand using requests.c which is an overlay   */
/*                    of this library.                   */
/*           It will parse the content-length,           */
/*         handle chunked connections, and so on         */
/*         https://github.com/mactul/requests.c          */

/*********************************************************/


int main()
{
    SocketHandler* client_handler;
    char buffer[1024];
    char headers[] = "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:106.0) Gecko/20100101 Firefox/106.0\r\n"
        "Connection: close\r\n\r\n";
    
    socket_start();  // This is for Windows compatibility, it do nothing on Linux.
    
    client_handler = socket_ssl_client_init("example.com", 443, NULL);

    if(client_handler == NULL)
    {
        socket_print_last_error();
        return 1;
    }

    socket_send(client_handler, headers, strlen(headers)+1, 0);

    while(socket_recv(client_handler, buffer, sizeof(buffer), 0) > 0)
        printf("%s\n", buffer);

    socket_close(&client_handler);  // this is very important because it will free all structures

    socket_cleanup();  // again, for Windows compatibility

    return 0;
}