#include <stdint.h>

typedef enum
{
    OBJECT_TYPE_STRING,
    OBJECT_TYPE_BOOLEAN,
    OBJECT_TYPE_I8,
    OBJECT_TYPE_I16,
    OBJECT_TYPE_I32,
    OBJECT_TYPE_I64,
    OBJECT_TYPE_U8,
    OBJECT_TYPE_U16,
    OBJECT_TYPE_U32,
    OBJECT_TYPE_U64,
    OBJECT_TYPE_F32,
    OBJECT_TYPE_F64,
    OBJECT_TYPE_ARRAY,
    OBJECT_TYPE_TUPLE,
} ObjectType;

typedef struct
{
    union
    {
        int16_t i16;
    } from;
} ObjectValue;

typedef struct
{
    ObjectType type;
    ObjectValue value;
} Object;

Object *Object_create(ObjectType type, uint64_t value);

Object *Object_create_with_value(ObjectType type, uint64_t value);

void Object_free(Object *object);

void Object_print(Object *object);

uint64_t Object_get_data_size(Object *object);

uint64_t Object_i16_get_value(Object *object);

uint64_t Object_get_value(Object *object);

uint8_t *convert_to_object_data(ObjectType type, uint64_t value);

Object *Object_equal(Object *object1, Object *object2);

Object *Object_less(Object *object1, Object *object2);

Object *Object_greater(Object *object1, Object *object2);

Object *Object_less_equal(Object *object1, Object *object2);

Object *Object_greater_equal(Object *object1, Object *object2);

Object *Object_add(Object *object1, Object *object2);

Object *Object_sub(Object *object1, Object *object2);

Object *Object_mul(Object *object1, Object *object2);

Object *Object_div(Object *object1, Object *object2);

Object *Object_bitwise_and(Object *object1, Object *object2);

Object *Object_bitwise_or(Object *object1, Object *object2);

Object *Object_bitwise_xor(Object *object1, Object *object2);

Object *Object_bitwise_not(Object *object);

Object *Object_bitwise_left_shift(Object *object1, Object *object2);

Object *Object_bitwise_right_shift(Object *object1, Object *object2);
