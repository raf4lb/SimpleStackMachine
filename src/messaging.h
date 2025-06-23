#include <stdint.h>
#include "sys.h"

#define MAX_PAYLOAD_SIZE 32
#define MAX_TOTAL_FRAGS 16 // depende da RAM disponível

typedef struct Message Message;
struct Message{
    struct Message* next;
    uint16_t vm_src;
    uint16_t vm_dst;
    uint16_t task_dst;
    uint16_t seq;
    uint8_t frag_id;
    uint8_t frag_total;
    uint16_t len;
    uint16_t crc;
    uint8_t *payload;
};

#define TASK_MESSAGE_QUEUE_SIZE 8

typedef struct MessageQueue MessageQueue;
struct MessageQueue{
    struct MessageQueue *next;
    uint16_t task_id;
    struct Message* head;
    struct Message* tail;
    uint16_t count;
};

int generate_sequence_id();

uint16_t compute_crc(Message* msg);

bool deliver_local(MessageQueue *local_task_inbox, Message *message);

void send_remote_message(uint16_t vm_id, Message* msg);

void send_message(MessageQueue *local_task_inbox, Message* msg);

// void send_large_message(MessageQueue *local_task_inbox, uint16_t vm_dst, uint16_t task_dst, const uint8_t* data, uint32_t full_len);

// void on_message_received(MessageQueue *local_task_inbox, const uint8_t* raw_data, uint16_t size);

MessageQueue *message_queue_create(uint16_t task_id);

void message_queue_free(MessageQueue *queue);