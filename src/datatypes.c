#include "datatypes.h"

uint8_t get_data_type_size(uint8_t data_type)
{
    if (data_type == TYPE_U16 || data_type == TYPE_I16)
    {
        return WORD;
    }
    else if (data_type == TYPE_U32 || data_type == TYPE_I32 || data_type == TYPE_F32)
    {
        return DWORD;
    }
    return 4;
}
