# EasyTCP-TLS
EasyTCP-TLS is a library that allows to easily make sockets connections, under Windows or Linux, cyphered in ssl or classic.

## Installation / Compilation

First of all, you have to install openssl.

If you are on Linux, just use apt (or wathever package manager you want).
```sh
sudo apt-get install openssl openssl-dev
```

On Windows, it's a bit trickier, I recommand to use vcpkg if you are using vscode.

Then, you have to link all the dependencies, which are `crypto` and `ssl`.\
On windows you also need `ws2_32`.

For compiling http_client.c for exemple, with gcc, on linux it's like that:
```sh
gcc http_client.c easy_tcp_tls.c -o http_client -I/openssl/* -lcrypto -lssl -Wall
```

On windows it's like that:
```sh
gcc http_client.c easy_tcp_tls.c -o http_client.exe -lws2_32 -I/openssl/* -lcrypto -lssl -Wall
```

## __Documentation__

### __Basic template__

All programs needs these 2 headers:
```c
#include <stdint.h>
#include "easy_tcp_tls.h"
```
You must absolutely include stdint before easy_tcp_tls, because easy_tcp_tls.h needs somes types defined in stdint.h

Then, you have to encapsulate your code between these 2 functions:
```c
socket_start();

// your code

socket_cleanup();
```
This is needed for windows compatibility.

**/!\\ Warning, on windows `socket_cleanup()` will destroy all the sockets, do not use it on a library for example**

### __Functions__

`void socket_start(void)`
- This function is used for windows compatibility and it will tell windows that the program will starts to use sockets.
- In theorie it can't fail.

`void socket_cleanup(void)`
- coupled with the function above, it will tell windows that the program doesn't need sockets anymore.
- In theorie, it can't fail.

`SocketHandler* socket_client_init(const char* server_hostname, uint16_t server_port)`
- This function will create the socket and returns a socket handler.
- **parameters:**
    - `server_hostname`: the targeted server host name, formated like "127.0.0.1", like "2001:0db8:85a3:0000:0000:8a2e:0370:7334" or like "example.com"
    - `server_port`: the opened server port that listen the connection
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `socket_print_last_error()` can tell what happened

`SocketHandler* socket_ssl_client_init(const char* server_hostname, uint16_t server_port, const char* sni_hostname)`
- This function works like socket_client_init, but it will create an ssl secured socket connexion.
- **parameters:**
    - `server_hostname`: the targeted server ip, formated like "127.0.0.1", like "2001:0db8:85a3:0000:0000:8a2e:0370:7334" or like "example.com"
    - `server_port`: the opened server port that listen the connection
    - `sni_hostname`: this is especially for web applications, when a single ip adress can have multiple services. If it's set to NULL, it will take the value of server_hostname, otherwise, you must set the domain name of the server.
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `socket_print_last_error()` can tell what happened

`SocketHandler* socket_server_init(const char* server_hostname, uint16_t server_port, int max_connections)`
- This function will create a server instance
- **parameters:**
    - `server_hostname`: this is always the hostname of the machine. For local applications it can be "127.0.0.1" or "localhost"
    - `server_port`: the port you want to use listen. make sure it's not already taken or you will have an `UNABLE_TO_BIND` error. If your server isn't local, make sure your port is opened on your firewall.
    - `max_connections`: The posix documentation is not realy clear with the role of this backlog parameter, but it provide a hint for the compiler. This parameter should not be used to physically limit a server.
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `socket_print_last_error()` can tell what happened

`SocketHandler* socket_ssl_server_init(const char* server_hostname, uint16_t server_port, int max_connections, const char* public_key_fp, const char* private_key_fp)`
- This function will create an ssl secured server instance
- You must generate a public and a private key with this command
    - `openssl req -x509 -newkey rsa:4096 -nodes -out ./cert.pem -keyout ./key.pem -days 365`
- **parameters:**
    - `server_hostname`: this is always the ip of the machine or the localhost ip: "127.0.0.1"
    - `server_port`: the port you want to use listen. make sure it's not already taken or you will have an `UNABLE_TO_BIND` error. If your server isn't local, make sure your port is opened on your firewall.
    - `max_connections`: The posix documentation is not realy clear with the role of this backlog parameter, but it provide a hint for the compiler. This parameter should not be used to physically limit a server.
    - `public_key_fp`: the path for your `cert.pem` file
    - `private_key_fp`: the path for your `key.pem` file
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `socket_print_last_error()` can tell what happened

`SocketHandler* socket_accept(SocketHandler* server, ClientData* pclient_data)`
- This function is used in a server application to wait and accept client connections.
- **parameters:**
    - `server`: a pointer to a SocketHandler, returned by - `socket_server_init` or `socket_ssl_server_init`
    - `pclient_data`: a pointer to a ClientData structure. It can be NULL if you don't care about client ip infos.
        - ClientData structure is defined like that:
            ```c
            typedef struct client_data {
                char ip[ADDRSTRLEN];
                uint16_t port;
            } ClientData;
            ```
        - If you have defined `ClientData infos;`, you can access `infos.ip` and `infos.port`.
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `socket_print_last_error()` can tell what happened

`int socket_send(SocketHandler* s, const char* buffer, int n, int flags)`
- This function will send the datas contained in the buffer array through the socket
- **parameters:**
    - `s`: a pointer to a SocketHandler. If you are in a client application, it's the handler returned by `socket_client_init` or `socket_ssl_client_init`. If you are in a server application, it's the handler returned by `socket_accept`
    - `buffer`: a buffer containing all the datas you want to send
    - `n`: the size of the datas, this can be different from the `sizeof(buffer)` if your buffer isn't full.
    - `flags`: I recommand you to let this parameter to `0`, but you can see the man page of `send` posix function to know more about that.
- **returns:**
    - when it succeeds, it returns the number of bytes sended
    - when it fails, it returns -1 and errno contains more information.

`int socket_recv(SocketHandler* s, char* buffer, int n, int flags)`
- This function will wait for data to arrive in the socket and fill a buffer with them.
- **parameters:**
    - `s`: a pointer to a SocketHandler. If you are in a client application, it's the handler returned by `socket_client_init` or `socket_ssl_client_init`. If you are in a server application, it's the handler returned by `socket_accept`
    - `buffer`: an empty buffer that will be filled with data from the socket 
    - `n`: the size of your buffer, you can simply provide `sizeof(buffer)`.
    - `flags`: I recommand you to let this parameter to `0`, but you can see the man page of `recv` posix function to know more about that.
- **returns:**
    - when it succeeds, it returns the number of bytes readed
    - when it fails, it returns -1 and errno contains more information.

`void socket_close(SocketHandler** pps)`
- This function take the address of the pointer on the handler to release all the stuff and put the Sockethandler pointer to NULL.
- **parameters:**
    - `pps`: the address of the pointer on the socket
- **returns:**
    - Nothing.
    - In theorie, if the pointer pps passed is coherent, this function can't fail.
    - If it creates a runtime error, maybe the memory behind the pointer is not allocated properly.

`int socket_get_last_error(void)`
- This function returns `0` if there is no relevant error knowned or a value defined `enum ERROR_CODES` if something has happened.
- This function is only relevant in production, to retrieve errors. For debugging, you can use `socket_print_last_error`

`socket_print_last_error`
- This function will print the last error knowed. You can call it to know what happened when a function returns NULL instead of a pointer.


### __Examples__

Here is a basic client example

```c
#include <stdint.h>  // this header have to be included before "easy_tcp_tls.h"
#include "easy_tcp_tls.h"

int main()
{
    SocketHandler* client_handler;  // This handler represents the socket
    char buffer[100] = "hello world";

    socket_start();  // This is for Windows compatibility, it do nothing on Linux.

    client_handler = socket_client_init("127.0.0.1", 3678);  // You can use socket_ssl_client_init if your server require an ssl connection.

    if(client_handler == NULL)
    {
        socket_print_last_error();
        return 1;
    }

    socket_send(client_handler, buffer, 15, 0);  // We just send the relevant content of our buffer, the 15 first bytes.

    socket_close(&client_handler);  // this is very important because it will free all structures

    socket_cleanup();  // again, for Windows compatibility
}
```

And is basic server correspondant.

```c
#include <stdint.h>  // this header have to be included before "easy_tcp_tls.h"
#include <stdio.h>
#include "easy_tcp_tls.h"


int main()
{
    char buffer[1024];
    SocketHandler* server_handler;
    SocketHandler* client_handler;
    ClientData infos;

    socket_start();  // This is for Windows compatibility, it do nothing on Linux.
    
    server_handler = socket_server_init("127.0.0.1", 3678, 1);  // You can use socket_ssl_server_init if you want the connection to be secure, but you have to create a cert.pem and a key.pem file.
    
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
```