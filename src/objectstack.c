#include "objectstack.h"
#include "io.h"
#include <stdint.h>
#include <stdlib.h>

void ObjectStack_error_print(char *message)
{
    vmprintf("ObjectStackError: %s\n", message);
}

ObjectStack *ObjectStack_create(uint16_t size)
{
    ObjectStack *stack = (ObjectStack *)malloc(sizeof(ObjectStack));
    if (stack == NULL)
    {
        ObjectStack_error_print("Memory allocation failed for ObjectStack\n");
        exit(EXIT_FAILURE);
    }
    stack->size = size;
    Object **data = (Object **)malloc(size * sizeof(Object *));
    if (data == NULL)
    {
        ObjectStack_error_print("Memory allocation failed for ObjectStack->data\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
    {
        data[i] = NULL;
    }
    stack->data = data;
    stack->sp = 0;
    return stack;
}

void ObjectStack_free(ObjectStack *stack)
{
    free(stack->data);
    free(stack);
}

Object *ObjectStack_pop(ObjectStack *stack)
{
    if (stack->sp <= 0)
    {
        ObjectStack_error_print("stackunderflow");
        exit(EXIT_FAILURE);
    }
    stack->sp--;
    return stack->data[stack->sp];
}

void ObjectStack_push(ObjectStack *stack, Object *object)
{
    if (stack->sp >= stack->size)
    {
        ObjectStack_error_print("stackoverflow");
        exit(EXIT_FAILURE);
    }
    stack->data[stack->sp++] = object;
}

void ObjectStack_print(ObjectStack *stack)
{
    vmprintf("[");
    for (int i = 0; i < stack->size; i++)
    {
        if (i > 0)
        {
            vmprintf(", ");
        }
        if (i == stack->sp)
        {
            vmprintf("->");
        }
        if (stack->data[i] == NULL)
        {
            vmprintf("0");
        }
        else
        {
            Object_print(stack->data[i]);
        }
    }
    vmprintf("]\n");
}
