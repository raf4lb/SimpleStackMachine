#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../src/messaging.h"

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "Usage: %s <message> <task_dst_id> <vm_dst_ip> <vm_dst_port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *payload = argv[1];
    size_t payload_len = strlen(payload);
    uint16_t task_dst_id = (uint16_t)atoi(argv[2]);
    const char *vm_dst_ip = argv[3];
    uint16_t vm_dst_port = (uint16_t)atoi(argv[4]);

    Message msg = {
        .vm_src = 0,
        .vm_dst = 1,
        .task_src = 0,
        .task_dst = task_dst_id,
        .seq = 0,
        .payload_size = payload_len,
        .crc = 0xBEEF,
        .frag_id = 0,
        .frag_total = 1,
        .payload = (uint8_t *)payload,
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
        .sin_port = htons(vm_dst_port)};
    if (inet_pton(AF_INET, vm_dst_ip, &server_addr.sin_addr) <= 0)
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
        printf("Message sent: %s\n", payload);
    }

    close(sock);
    free(serialized);
    return 0;
}
