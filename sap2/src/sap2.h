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
#define SAP_RAM_SIZE 0xFFFF

// 8Bit Opcode Definitions:
#define OPCODE_MASK 0xF0
#define OPERAND_MASK 0x0F

#define OPCODE_NOP 0x00 // No Execution
#define OPCODE_HLT 0x76 // Halt Instruction Flow

#define OPCODE_MVI_A 0x3E // Move the Subsequent Byte Value into Register (A|B|C). 2 Byte Instruction
#define OPCODE_MVI_B 0x06
#define OPCODE_MVI_C 0x0E

#define OPCODE_LDA 0x3A // Load the following 16Bit (2 Byte) Memory Address into the Accumulator. 3 Byte Instruction.
#define OPCODE_STA 0x32 // Store the Accumulator into the following 16Bit (2 Byte) Memory Address. 3 Byte Instruction.

#define OPCODE_JMP 0xC3 // Unconditional Jump
#define OPCODE_JZ  0xCA // Unconditional Jump

#define OPCODE_CALL 0xCD // Call Function
#define OPCODE_RET  0xC9 // Return to Caller

#define OPCODE_ADD_B 0x80 // Add the value in Register (B|C) to Accumulator
#define OPCODE_ADD_C 0x81

#define OPCODE_SUB_B 0x90 // Substract the value in Reg (B|C) from Accumulator
#define OPCODE_SUB_C 0x91

#define OPCODE_INR_A 0x3C // Increment Registers by 1:
#define OPCODE_INR_B 0x04
#define OPCODE_INR_C 0x0C

#define OPCODE_DCR_A 0x3D // Decrement Registers by 1:
#define OPCODE_DCR_B 0x05
#define OPCODE_DCR_C 0x0D

#define OPCODE_MOV_A_B 0x78 // Move from Reg X to Reg Y
#define OPCODE_MOV_A_C 0x79
#define OPCODE_MOV_B_A 0x47
#define OPCODE_MOV_B_C 0x41
#define OPCODE_MOV_C_A 0x4F
#define OPCODE_MOV_C_B 0x48

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
