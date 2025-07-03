#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../src/messaging.h"

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <task_dst> <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    uint16_t task_dst = (uint16_t)atoi(argv[1]);
    const char *text = argv[2];
    size_t payload_len = strlen(text);

    Message msg = {
        .vm_src = 1,
        .vm_dst = 2,
        .task_src = 0,
        .task_dst = task_dst,
        .seq = 1234,
        .payload_size = payload_len,
        .crc = 0xBEEF,
        .frag_id = 0,
        .frag_total = 1,
        .payload = (uint8_t *)text,
    };

    size_t serialized_len;
    uint8_t *serialized = message_serialize(&msg, &serialized_len);
    if (!serialized)
    {
        fprintf(stderr, "Error serializing message\n");
        return EXIT_FAILURE;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        free(serialized);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT)};
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sock);
        free(serialized);
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        close(sock);
        free(serialized);
        return EXIT_FAILURE;
    }

    ssize_t sent = send(sock, serialized, serialized_len, 0);
    if (sent < 0)
    {
        perror("send");
    }
    else
    {
        printf("Message sent (%ld bytes) to task %u: %s\n", sent, task_dst, text);
    }

    close(sock);
    free(serialized);
    return 0;
}
