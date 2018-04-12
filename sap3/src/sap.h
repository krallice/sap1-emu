#ifndef SAP_H
#define SAP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//typedef struct sap1_state_t sap_state_t;

// Enable Debug Messages:
#define SAP_DEBUG 1

// n 8Bit RAM Addresses:
#define SAP_RAM_SIZE 0xFFFF

// 8Bit Opcode Definitions:
#define OPCODE_MASK 0xF0
#define OPERAND_MASK 0x0F

// Struct for SAP-2:
typedef struct sap3_state_t {
	uint16_t pc; // 16Bit Program Counter

        int8_t a; // 8Bit A Register
        int8_t b; // 8Bit B Register
	int8_t c; // 8Bit C Register

	uint8_t flag_sign :1; // 1Bit Sign Flag - Set when the Accumulator becomes <0
	uint8_t flag_zero :1; // 1Bit Zero Flag - Set when the Accumulator becomes 0

	uint8_t *ram; // Pointer to ram_size x 8bit RAM Words
	uint16_t ram_size;
} sap_state_t;

sap_state_t *init_sap_state(void);

// Dump Memory:
void dump_sap_memory(sap_state_t *sap_state);

// Dump Structure State:
void dump_sap_state(sap_state_t *sap_state);

void execute_sap(sap_state_t *sap_state);
#endif
