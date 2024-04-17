#include <stdlib.h>
#include "object.h"
#include "io.h"

void Object_error_print(char *message)
{
    vmprintf("ObjectError: %s", message);
}

uint64_t ObjectType_get_size(ObjectType type)
{
    switch (type)
    {
    case OBJECT_TYPE_I16:
        return 2;
        break;

    default:
        return 0;
        break;
    }
}

Object *Object_create(ObjectType type, uint64_t value)
{
    Object *object = (Object *)malloc(sizeof(Object));
    if (object == NULL)
    {
        Object_error_print("Memory allocation failed for Object\n");
        exit(EXIT_FAILURE);
    }
    object->type = type;
    switch (type)
    {
    case OBJECT_TYPE_I16:
        object->value.from.i16 = (int16_t)value;
        break;

    default:
        break;
    }

    return object;
}

// Object *Object_create_with_value(ObjectType type, uint64_t value)
// {
//     uint8_t *data = convert_to_object_data(type, value);
//     return Object_create(type, data);
// }

void Object_free(Object *object)
{
    free(object);
}

void Object_print(Object *object)
{
    vmprintf("<%d: %d>", object->type, object->value.from.i16);
}

uint64_t Object_get_data_size(Object *object)
{
    switch (object->type)
    {
    case OBJECT_TYPE_I16:
        return 16;
        break;

    default:
        return 0;
        break;
    }
}

uint8_t *convert_to_object_data(ObjectType type, uint64_t value)
{
    uint8_t size = ObjectType_get_size(type);
    uint8_t *data = (uint8_t *)malloc(size * sizeof(uint8_t));
    if (data == NULL)
    {
        vmprintf("ObjectError: Error converting object type %d data %llu\n", type, data);
        exit(EXIT_FAILURE);
    }
    for (uint8_t i = 0; i < size; i++)
    {
        data[i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
    return data;
}

// uint64_t Object_i16_get_value(Object *object)
// {
//     uint64_t value = object->data[0] << 8 | object->data[1];
//     return value;
// }

// uint64_t Object_get_value(Object *object)
// {
//     switch (object->type)
//     {
//     case OBJECT_TYPE_I16:
//         return Object_i16_get_value(object);
//         break;

//     default:
//         return 0;
//         break;
//     }
// }

// TODO: Reduce duplicate code

Object *Object_equal(Object *object1, Object *object2)
{
    // TODO: Make true and false constant and static
    Object *object_result;
    if (object1->type == object2->type)
    {
        uint8_t result = (uint8_t)(object1->value.from.i16 == object2->value.from.i16);
        object_result = Object_create(OBJECT_TYPE_BOOLEAN, result);
    }
    else
    {
        object_result = Object_create(OBJECT_TYPE_BOOLEAN, 0);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_less(Object *object1, Object *object2)
{
    // TODO: Make true and false constant and static
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            uint8_t result = (uint8_t)(object1->value.from.i16 < object2->value.from.i16);
            object_result = Object_create(OBJECT_TYPE_BOOLEAN, result);
        }
    }
    else
    {
        Object_error_print("cannot compare objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_greater(Object *object1, Object *object2)
{
    // TODO: Make true and false constant and static
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            uint8_t result = (uint8_t)(object1->value.from.i16 > object2->value.from.i16);
            object_result = Object_create(OBJECT_TYPE_BOOLEAN, result);
        }
    }
    else
    {
        Object_error_print("cannot compare objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_less_equal(Object *object1, Object *object2)
{
    // TODO: Make true and false constant and static
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            uint8_t result = (uint8_t)(object1->value.from.i16 <= object2->value.from.i16);
            object_result = Object_create(OBJECT_TYPE_BOOLEAN, result);
        }
    }
    else
    {
        Object_error_print("cannot compare objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_greater_equal(Object *object1, Object *object2)
{
    // TODO: Make true and false constant and static
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            uint8_t result = (uint8_t)(object1->value.from.i16 >= object2->value.from.i16);
            object_result = Object_create(OBJECT_TYPE_BOOLEAN, result);
        }
    }
    else
    {
        Object_error_print("cannot compare objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_add(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 + object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot add objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_sub(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 - object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot subtract objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_mul(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 * object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot multiply objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_div(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 / object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot divide objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_bitwise_and(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 & object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot bitwise and objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_bitwise_or(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 | object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot bitwise or objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_bitwise_xor(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 ^ object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot bitwise xor objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_bitwise_not(Object *object)
{
    if (object->type == OBJECT_TYPE_I16)
    {
        int16_t result = ~object->value.from.i16;
        Object *object_result = Object_create(OBJECT_TYPE_I16, result);
        Object_free(object);
        return object_result;
    }
    Object_error_print("cannot bitwise not object");
    exit(EXIT_FAILURE);
}

Object *Object_bitwise_left_shift(Object *object1, Object *object2)
{
    Object *object_result;
    vmprintf("object1 type %d; object2 type %d\n", object1->type, object2->type);
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 << object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot left shift objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}

Object *Object_bitwise_right_shift(Object *object1, Object *object2)
{
    Object *object_result;
    if (object1->type == object2->type)
    {
        if (object1->type == OBJECT_TYPE_I16)
        {
            int16_t result = object1->value.from.i16 >> object2->value.from.i16;
            object_result = Object_create(OBJECT_TYPE_I16, result);
        }
    }
    else
    {
        Object_error_print("cannot right shift objects of different types");
        exit(EXIT_FAILURE);
    }
    Object_free(object1);
    Object_free(object2);
    return object_result;
}
