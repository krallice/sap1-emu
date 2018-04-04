#ifndef SAP1_H
#define SAP1_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//typedef struct sap1_state_t sap_state_t;

// Enable Debug Messages:
#define SAP_DEBUG 1

// Amount of 8bit RAM Addresses - Design calls for 4bit (16):
#define SAP_RAM_SIZE 16

// Opcode Definitions:
#define OPCODE_HLT 0xF

// Struct for SAP-1:
typedef struct sap1_state_t {
	uint8_t pc :4; // 4Bit Program Counter (16 Values)
        uint8_t a; // 8Bit A Register
        uint8_t b; // 8Bit B Register
	uint8_t *ram; // Pointer to n x 8bit RAM Addresses
	uint8_t ram_size;
} sap_state_t;

sap_state_t *init_sap_state(void);

// Dump Memory:
void dump_sap_memory(sap_state_t *sap_state);

void execute_sap(sap_state_t *sap_state);
#endif
