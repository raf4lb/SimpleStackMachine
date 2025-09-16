#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "io.h"
#include "messaging.h"
#include "string.h"

int generate_sequence_id()
{
    return 1;
}

uint16_t compute_crc(Message *message)
{
    uint16_t crc = 0;
    for (int i = 0; i < message->payload_size; i++)
    {
        crc ^= message->payload[i];
    }
    return crc;
}

bool deliver_local(MessageQueue *message_queues, Message *message)
{
    MessageQueue *queue = message_queues;
    while (queue != NULL)
    {
        if (queue->task_id == message->task_dst)
        {
            if (queue->count >= TASK_MESSAGE_QUEUE_SIZE)
            {
                vmprintf("task %d inbox is full\n", message->task_dst);
                return false;
            }
            if (queue->head == NULL)
            {
                queue->head = message;
                queue->tail = message;
            }
            else
            {
                queue->tail->next = message;
                queue->tail = message;
            }

            queue->count++;
            return true;
        }
        queue = queue->next;
    }
    vmprintf("task %d not found\n", message->task_dst);
    message_free(message);
    return false;
}

// Função genérica que envia para outra VM (por UDP/TCP/serial/etc)
void send_remote_message(uint16_t vm_id, Message *msg)
{
    vmprintf("Sending message to remote vm %d\n", vm_id);
}

void send_message(MessageQueue *local_task_inbox, Message *message)
{
    if (message == NULL)
    {
        vmprintf("message is NULL\n");
        return;
    }
    if (message->vm_dst == get_local_vm_id())
    {
        if (!deliver_local(local_task_inbox, message))
        {
            vmprintf("Error delivering local message\n");
        }
    }
    else
    {
        send_remote_message(message->vm_dst, message);
    }
}

// void send_large_message(MessageQueue *local_task_inbox, uint16_t vm_dst, uint16_t task_dst, const uint8_t *data, uint32_t full_len)
// {
//     uint16_t seq = generate_sequence_id(); // deve ser único por mensagem
//     uint8_t total_frags = (full_len + MAX_PAYLOAD_SIZE - 1) / MAX_PAYLOAD_SIZE;

//     for (uint8_t i = 0; i < total_frags; i++)
//     {
//         Message frag = {0};
//         frag.vm_src = get_local_vm_id();
//         frag.vm_dst = vm_dst;
//         frag.task_dst = task_dst;
//         frag.seq = seq;
//         frag.frag_id = i;
//         frag.frag_total = total_frags;

//         uint16_t offset = i * MAX_PAYLOAD_SIZE;
//         uint16_t this_len = (full_len - offset > MAX_PAYLOAD_SIZE) ? MAX_PAYLOAD_SIZE : (full_len - offset);

//         frag.len = this_len;
//         memcpy(frag.payload, data + offset, this_len);
//         frag.crc = compute_crc(&frag);

//         send_message(local_task_inbox, &frag);
//     }
// }

MessageQueue *message_queue_create(uint16_t task_id)
{
    MessageQueue *queue = (MessageQueue *)vmmalloc(sizeof(MessageQueue));
    if (queue == NULL)
    {
        vmprintf("mem_alloc_failed: message queue\n");
        exit(EXIT_FAILURE);
    }
    queue->task_id = task_id;
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
    queue->next = NULL;
    return queue;
}

void message_queue_free(MessageQueue *queue)
{
    Message *message = queue->head;
    while (message != NULL)
    {
        Message *next = message->next;
        message_free(message);
        message = next;
    }
    vmfree(queue, sizeof(MessageQueue));
}

Message *message_create(uint16_t vm_src, uint16_t vm_dst, uint16_t task_src_id, uint16_t task_dst_id, uint8_t *payload, uint16_t payload_size)
{
    Message *message = (Message *)vmmalloc(sizeof(Message));
    message->frag_id = 0;
    message->frag_total = 0;
    message->seq = 0;
    message->vm_dst = vm_dst;
    message->vm_src = vm_src;
    message->task_src = task_src_id;
    message->task_dst = task_dst_id;
    message->payload_size = payload_size;
    message->payload = (uint8_t *)vmmalloc(payload_size * sizeof(uint8_t));
    for (uint16_t i = 0; i < payload_size; i++)
    {
        message->payload[i] = payload[i];
    }
    message->crc = compute_crc(message);
    return message;
}

void message_free(Message *message)
{
    vmfree(message->payload, sizeof(uint8_t) * message->payload_size);
    vmfree(message, sizeof(Message));
}

void message_queue_send_message(MessageQueue *message_queue, uint16_t task_src_id, uint16_t task_dst_id, uint8_t *payload, uint16_t payload_size)
{
    uint16_t vm_src = get_local_vm_id();
    Message *message = message_create(vm_src, vm_src, task_src_id, task_dst_id, payload, payload_size);
    send_message(message_queue, message);
}

uint8_t *message_serialize(const Message *msg, size_t *out_size)
{
    *out_size = 16 + msg->payload_size;
    uint8_t *buffer = vmmalloc(*out_size);
    if (!buffer)
    {
        vmprintf("mem_alloc_failed: message buffer\n");
        return NULL;
    }

    uint8_t *ptr = buffer;

    memcpy(ptr, &msg->vm_src, 2);
    ptr += 2;
    memcpy(ptr, &msg->vm_dst, 2);
    ptr += 2;
    memcpy(ptr, &msg->task_src, 2);
    ptr += 2;
    memcpy(ptr, &msg->task_dst, 2);
    ptr += 2;
    memcpy(ptr, &msg->seq, 2);
    ptr += 2;
    memcpy(ptr, &msg->payload_size, 2);
    ptr += 2;
    memcpy(ptr, &msg->crc, 2);
    ptr += 2;
    *ptr++ = msg->frag_id;
    *ptr++ = msg->frag_total;

    if (msg->payload_size > 0 && msg->payload)
    {
        memcpy(ptr, msg->payload, msg->payload_size);
    }

    return buffer;
}

Message *message_deserialize(const uint8_t *buffer, size_t buffer_size)
{
    if (buffer_size < 16)
    {
        return NULL;
    }

    Message *msg = vmcalloc(1, sizeof(Message));
    if (!msg)
    {
        vmprintf("mem_alloc_failed: message buffer\n");
        return NULL;
    }

    const uint8_t *ptr = buffer;

    memcpy(&msg->vm_src, ptr, 2);
    ptr += 2;
    memcpy(&msg->vm_dst, ptr, 2);
    ptr += 2;
    memcpy(&msg->task_src, ptr, 2);
    ptr += 2;
    memcpy(&msg->task_dst, ptr, 2);
    ptr += 2;
    memcpy(&msg->seq, ptr, 2);
    ptr += 2;
    memcpy(&msg->payload_size, ptr, 2);
    ptr += 2;
    memcpy(&msg->crc, ptr, 2);
    ptr += 2;
    msg->frag_id = *ptr++;
    msg->frag_total = *ptr++;

    if (msg->payload_size > 0 && (ptr + msg->payload_size <= buffer + buffer_size))
    {
        msg->payload = vmmalloc(msg->payload_size);
        if (!msg->payload)
        {
            vmprintf("mem_alloc_failed: message payload\n");
            message_free(msg);
            return NULL;
        }
        memcpy(msg->payload, ptr, msg->payload_size);
    }

    return msg;
}
