# EasyTCP-TLS
EasyTCP-TLS is a library that allows to easily make sockets connections, under Windows or Linux, encrypted in ssl or classic.

Known bugs:
In ssl connection, if the socket connection is closed too early, the socket_send has no time to send its data.
I have to make this function completely blocking.