#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#include "messaging.h"

#define SERVER_PORT 31337

typedef struct
{
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen;
    int port;
} Server;

Server *server_create(int port);
Message *server_receive_message(Server *server);
void server_free(Server *server);

#endif