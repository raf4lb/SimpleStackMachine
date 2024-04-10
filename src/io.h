#ifndef IO_H
#define IO_H

#define _DDRB 0
#define _PORTB 1
#include <stdint.h>

typedef struct
{
    volatile uint8_t **ports;
    uint8_t size;
} PortBank;

void map_ports(PortBank *port_bank);

PortBank *port_bank_create(uint8_t size);

void port_bank_print(PortBank *port_bank);

void port_bank_free(PortBank *port_bank);

void port_bank_set_address(PortBank *port_bank, uint8_t address, uint8_t value);

uint8_t port_bank_get_address(PortBank *port_bank, uint8_t address);

#endif