#include <stdio.h>
#include "../src/objectstack.h"
#include "../src/io.h"

uint16_t stack_size = 16;

void print_result(char *test_name, int failed)
{
    if (failed)
    {
        vmprintf("%s FAILED\n", test_name);
    }
    else
    {
        printf("%s SUCCESS\n", test_name);
    }
}

int test_objectstack_create(void)
{
    ObjectStack *stack = ObjectStack_create(stack_size);
    char failed = 0;

    if (stack->size != stack_size)
    {
        vmprintf("objectstack size error\n");
        failed = 1;
    }
    if (stack->sp != 0)
    {
        vmprintf("objectstack pointer error\n");
        failed = 1;
    }
    print_result("test_objectstack_create", failed);
    ObjectStack_free(stack);
    return failed;
}

int test_objectstack_push()
{
    ObjectStack *stack = ObjectStack_create(stack_size);
    uint8_t data[] = {1, 0};
    Object *object = Object_create(OBJECT_TYPE_I16, data);
    ObjectStack_push(stack, object);
    char failed = 0;

    if (stack->data[0]->type != OBJECT_TYPE_I16)
    {
        vmprintf("ObjectStack push error\n");
        failed = 1;
    }
    for (int i; i < 2; i++)
    {
        if (stack->data[0]->data[i] != data[i])
        {
            vmprintf("Object data error\n");
            failed = 1;
        }
    }
    if (stack->sp != 1)
    {
        vmprintf("ObjectStack pointer error\n");
        failed = 1;
    }

    print_result("test_objectstack_push", failed);
    ObjectStack_free(stack);
    Object_free(object);
    return failed;
}

int test_objectstack_pop()
{
    ObjectStack *stack = ObjectStack_create(stack_size);
    uint8_t data[] = {1, 0};
    Object *object = Object_create(OBJECT_TYPE_I16, data);

    stack->data[0] = object;
    stack->sp = 1;
    Object *object_popped = ObjectStack_pop(stack);

    char failed = 0;
    if (object_popped != object)
    {
        vmprintf("ObjectStack pop error\n");
        failed = 1;
    }
    if (stack->sp != 0)
    {
        vmprintf("ObjectStack pointer error\n");
        failed = 1;
    }

    print_result("test_objectstack_pop", failed);
    ObjectStack_free(stack);
    Object_free(object);
    return failed;
}

int main(void)
{
    int failed;
    failed = test_objectstack_create();
    failed = test_objectstack_push();
    failed = test_objectstack_pop();
    return failed;
}