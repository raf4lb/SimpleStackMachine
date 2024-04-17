#ifndef OBJECTSTACK_H
#define OBJECTSTACK_H

#include "object.h"

typedef struct
{
    Object **data;
    uint16_t size;
    uint16_t sp;
} ObjectStack;

ObjectStack *ObjectStack_create(uint16_t size);

void ObjectStack_free(ObjectStack *stack);

Object *ObjectStack_pop(ObjectStack *stack);

void ObjectStack_push(ObjectStack *stack, Object *object);

void ObjectStack_print(ObjectStack *stack);

#endif