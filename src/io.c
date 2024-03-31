#include "avr/io.h"
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "serial.h"

void map_ports(PortBank *port_bank)
{
    port_bank->ports[0] = &DDRB;
    port_bank->ports[1] = &PORTB;
    port_bank->ports[2] = &PINB;
}

PortBank *port_bank_create(uint8_t size)
{
    PortBank *port_bank = (PortBank *)malloc(sizeof(PortBank));
    if (port_bank == NULL)
    {
        serial_printf("Memory allocation failed for port bank\n");
        exit(1);
    }
    port_bank->size = size;
    port_bank->ports = (volatile uint8_t **)malloc(size * sizeof(volatile uint8_t *));
    if (port_bank->ports == NULL)
    {
        serial_printf("Memory allocation failed for ports of the bank\n");
        exit(1);
    }
    map_ports(port_bank);
    return port_bank;
}

void port_bank_print(PortBank *port_bank)
{
    serial_printf("[");
    for (int i = 0; i < port_bank->size; i++)
    {
        if (i > 0)
            serial_printf(", ");
        serial_printf("%hhu", *(port_bank->ports[i]));
    }
    serial_printf("]\n");
}

void port_bank_free(PortBank *port_bank)
{
    free(port_bank->ports);
    free(port_bank);
}

void port_bank_set_address(PortBank *port_bank, uint8_t address, uint8_t value)
{
    *(port_bank->ports[address]) = (uint8_t)value;
}

uint8_t port_bank_get_address(PortBank *port_bank, uint8_t address)
{
    return *(port_bank->ports[address]);
}