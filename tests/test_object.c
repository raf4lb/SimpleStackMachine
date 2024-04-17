#include <stdio.h>
#include <stdlib.h>
#include "../src/object.h"
#include "../src/io.h"

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

int test_object_create(void)
{
    int failed = 0;

    uint8_t data[] = {0, 1};
    Object *object = Object_create(OBJECT_TYPE_I16, data);

    if (object->type != OBJECT_TYPE_I16)
    {
        vmprintf("object type error\n");
        failed = 1;
    }

    if (object->data[0] != 0 || object->data[1] != 1)
    {
        vmprintf("object data error\n");
        failed = 1;
    }

    print_result("test_object_create", failed);
    Object_free(object);
    return failed;
}

int test_object_value(void)
{
    int failed;

    uint8_t data[] = {1, 0};
    Object *object = Object_create(OBJECT_TYPE_I16, data);

    int16_t value = (int16_t)Object_get_value(object);
    if (value != 256)
    {
        vmprintf("object value error: %d\n", value);
        failed = 1;
    }

    print_result("test_object_value", failed);
    Object_free(object);
    return failed;
}

int test_convert_to_object_data(void)
{
    int failed = 0;
    int16_t value = 1000;
    uint8_t *converted = convert_to_object_data(OBJECT_TYPE_I16, value);
    if (converted[0] != 3 || converted[1] != 232)
    {
        vmprintf("object data error\n");
        failed = 1;
    }
    free(converted);
    print_result("test_convert_to_object_data", failed);
    return failed;
}

int test_object_add_i16(void)
{
    int failed;

    uint8_t data1[] = {3, 232}; // 1000
    Object *object1 = Object_create(OBJECT_TYPE_I16, data1);
    uint8_t data2[] = {1, 244}; // 500
    Object *object2 = Object_create(OBJECT_TYPE_I16, data2);

    Object *result = Object_add(object1, object2); // 1500

    int16_t value = Object_get_value(result);
    if (value != 1500)
    {
        vmprintf("object value error: %d\n", value);
        failed = 1;
    }

    print_result("test_object_add", failed);
    Object_free(object1);
    Object_free(object2);
    return failed;
}

int test_object_sub_i16(void)
{
    int failed;

    uint8_t data1[] = {3, 232}; // 1000
    Object *object1 = Object_create(OBJECT_TYPE_I16, data1);
    uint8_t data2[] = {1, 244}; // 500
    Object *object2 = Object_create(OBJECT_TYPE_I16, data2);

    Object *result = Object_sub(object1, object2); // 500

    int16_t value = Object_get_value(result);
    if (value != 500)
    {
        vmprintf("object value error: %d\n", value);
        failed = 1;
    }

    print_result("test_object_sub", failed);
    Object_free(object1);
    Object_free(object2);
    return failed;
}

int main(void)
{
    int failed;
    failed = test_object_create();
    failed = test_object_value();
    failed = test_convert_to_object_data();
    failed = test_object_add_i16();
    failed = test_object_sub_i16();
    return failed;
}
