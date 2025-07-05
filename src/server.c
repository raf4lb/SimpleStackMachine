#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "io.h"
#include "server.h"
#include "sys.h"
#include "messaging.h"

#define BUFFER_SIZE 128

static int make_socket_non_blocking(int sfd)
{
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1)
        return -1;
    return fcntl(sfd, F_SETFL, flags | O_NONBLOCK);
}

Server *server_create(int port)
{
    Server *server = vmmalloc(sizeof(Server));
    if (!server)
    {
        vmprintf("mem_alloc_failed: server\n");
        exit(EXIT_FAILURE);
    }

    server->port = port;
    server->addrlen = sizeof(server->address);

    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_fd == -1)
    {
        vmprintf("socket error\n");
        vmfree(server, sizeof(Server));
        return NULL;
    }

    if (make_socket_non_blocking(server->server_fd) == -1)
    {
        vmprintf("fcntl error\n");
        server_free(server);
        return NULL;
    }

    memset(&server->address, 0, sizeof(server->address));
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);

    if (bind(server->server_fd, (struct sockaddr *)&server->address, server->addrlen) < 0)
    {
        vmprintf("bind error\n");
        server_free(server);
        return NULL;
    }

    if (listen(server->server_fd, 1) < 0)
    {
        vmprintf("listen error\n");
        server_free(server);
        return NULL;
    }

    return server;
}

Message *server_receive_message(Server *server)
{
    int client_fd = accept(server->server_fd, (struct sockaddr *)&server->address, &server->addrlen);
    if (client_fd < 0)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return NULL;
        }
        else
        {
            vmprintf("accept error\n");
            return NULL;
        }
    }

    // make client unblocking
    fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) & ~O_NONBLOCK);

    uint8_t buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);

    close(client_fd);

    if (bytes_read <= 0)
    {
        vmprintf("read error\n");
        return NULL;
    }

    return message_deserialize(buffer, (size_t)bytes_read);
}

void server_free(Server *server)
{
    if (server)
    {
        close(server->server_fd);
        vmfree(server, sizeof(Server));
    }
}
