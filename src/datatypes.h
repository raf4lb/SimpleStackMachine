#include <stdint.h>

typedef enum
{
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_F32,
    TYPE_F64,
} DATA_TYPES;

#define BYTE 1
#define WORD 2
#define DWORD 4
#define DDWORD 8

uint8_t get_data_type_size(uint8_t data_type);