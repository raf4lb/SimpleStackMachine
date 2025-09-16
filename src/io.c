#include "sys.h"
#include "io.h"

#include <stdlib.h>

#ifdef ARDUINO

int (*vmprintf)(const char *, ...) = serial_printf;

#elif defined(MACOSX) || defined(WINDOWS)

volatile uint8_t DDRB;
volatile uint8_t PORTB;
volatile uint8_t PINB;

int (*vmprintf)(const char *, ...) = printf;

#endif

void map_ports(PortBank *port_bank)
{
    port_bank->ports[0] = &DDRB;
    port_bank->ports[1] = &PORTB;
    port_bank->ports[2] = &PINB;
}

PortBank *port_bank_create(uint8_t size)
{
    PortBank *port_bank = (PortBank *)vmmalloc(sizeof(PortBank));
    if (port_bank == NULL)
    {
        vmprintf("mem_alloc_failed: port bank\n");
        exit(EXIT_FAILURE);
    }
    port_bank->size = size;
    port_bank->ports = (volatile uint8_t **)vmmalloc(size * sizeof(volatile uint8_t *));
    if (port_bank->ports == NULL)
    {
        vmprintf("mem_alloc_failed: ports of the bank\n");
        exit(EXIT_FAILURE);
    }
    map_ports(port_bank);
    return port_bank;
}

void port_bank_print(PortBank *port_bank)
{
    vmprintf("[");
    for (int i = 0; i < port_bank->size; i++)
    {
        if (i > 0)
            vmprintf(", ");
        vmprintf("%hhu", *(port_bank->ports[i]));
    }
    vmprintf("]\n");
}

void port_bank_free(PortBank *port_bank)
{
    vmfree(port_bank->ports, sizeof(uint8_t) * port_bank->size);
    vmfree(port_bank, sizeof(PortBank));
}

void port_bank_set_address(PortBank *port_bank, uint8_t address, uint8_t value)
{
    *(port_bank->ports[address]) = (uint8_t)value;
}

uint8_t port_bank_get_address(PortBank *port_bank, uint8_t address)
{
    return *(port_bank->ports[address]);
}