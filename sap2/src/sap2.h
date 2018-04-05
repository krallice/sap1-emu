#ifndef SAP2_H
#define SAP2_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//typedef struct sap1_state_t sap_state_t;

// Enable Debug Messages:
#define SAP_DEBUG 1

// n 8Bit RAM Addresses:
#define SAP_RAM_SIZE 128

// 8Bit Opcode Definitions:
#define OPCODE_MASK 0xF0
#define OPERAND_MASK 0x0F

#define OPCODE_HLT 0x76 //Halt Instruction Flow

#define OPCODE_LDA 0x0 //Load memory address into Accumulator
#define OPCODE_ADD 0x1 //Add memory address (operand) to Accumulator
#define OPCODE_SUB 0x2 //Subtract memory address (operand) from Accumulator
#define OPCODE_OUT 0xE //Send Accumulator Value to Output Pin

// Struct for SAP-2:
typedef struct sap1_state_t {
	uint16_t pc; // 16Bit Program Counter

        uint8_t a; // 8Bit A Register
        uint8_t b; // 8Bit B Register
	uint8_t c; // 8Bit C Register
	uint8_t tmp; // 8Bit TMP Register

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
