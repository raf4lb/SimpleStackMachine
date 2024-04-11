#include <stdio.h>
#include "../src/stack.h"
#include "../src/io.h"

uint16_t stack_size = 16;

void print_result(char *test_name, int failed)
{
    if (failed)
    {
        vmprintf(stderr, "%s FAILED\n", test_name);
    }
    else
    {
        printf("%s SUCCESS\n", test_name);
    }
}

int test_stack_create(void)
{
    Stack *stack = stack_create(stack_size);
    char failed = 0;

    if (stack->size != stack_size)
    {
        vmprintf(stderr, "stack size error\n");
        failed = 1;
    }
    if (stack->sp != 0)
    {
        vmprintf(stderr, "stack pointer error\n");
        failed = 1;
    }

    uint16_t i;
    for (i = 0; i < stack_size; i++)
    {
        if (stack->data[i] != 0)
        {
            vmprintf(stderr, "stack data size error\n");
            failed = 1;
            break;
        }
    }
    print_result("test_stack_create", failed);
    stack_free(stack);
    return failed;
}

int test_stack_push()
{
    Stack *stack = stack_create(stack_size);
    stack_push(stack, 10);
    char failed = 0;

    if (stack->data[0] != 10)
    {
        vmprintf(stderr, "stack push error\n");
        failed = 1;
    }
    if (stack->sp != 1)
    {
        vmprintf(stderr, "stack pointer error\n");
        failed = 1;
    }

    print_result("test_stack_push", failed);
    return failed;
}

int test_stack_pop()
{
    Stack *stack = stack_create(stack_size);
    stack->data[0] = 10;
    stack->sp = 1;
    uint8_t value = stack_pop(stack);

    char failed = 0;
    if (value != 10)
    {
        vmprintf(stderr, "stack pop error\n");
        failed = 1;
    }
    if (stack->sp != 0)
    {
        vmprintf(stderr, "stack pointer error\n");
        failed = 1;
    }

    print_result("test_stack_pop", failed);
    return failed;
}

int test_stack_push_bytes()
{
    char failed = 0;
    Stack *stack = stack_create(stack_size);
    uint8_t bytes[] = {10, 20, 30, 40};
    stack_push_bytes(stack, bytes, 4);
    if (stack->sp != 4)
    {
        vmprintf(stderr, "stack pointer error\n");
        failed = 1;
    }
    if (stack->data[0] != 10 || stack->data[1] != 20 || stack->data[2] != 30 || stack->data[3] != 40)
    {
        vmprintf(stderr, "stack data error\n");
        failed = 1;
    }
    print_result("test_stack_push_bytes", failed);
    return failed;
}

int main(void)
{
    int failed;
    failed = test_stack_create();
    failed = test_stack_push();
    failed = test_stack_pop();
    failed = test_stack_push_bytes();
    return failed;
}