#include "main.h"
#include "sap2.h"

int main(void) {

	// Init our SAP Microcontroller:
	sap_state_t *sap_state = init_sap_state();
	if (sap_state == NULL) {
		return 1;
	}

	// Program Memory:
	// Todo: Hide this exposure and implement through a propper API.
	// This is more akin to flipping bits with a magnetised needle

	// Operations:
	/*
	sap_state->ram[0x0000] = OPCODE_MVI_B;
	sap_state->ram[0x0001] = 0x20;

	sap_state->ram[0x0002] = OPCODE_MVI_C;
	sap_state->ram[0x0003] = 0x30;

	sap_state->ram[0x0004] = OPCODE_STA;
	sap_state->ram[0x0005] = 0x15;
	sap_state->ram[0x0006] = 0x00;

	sap_state->ram[0x0007] = OPCODE_ADD_B;

	sap_state->ram[0x0008] = OPCODE_INR_C;
	sap_state->ram[0x0009] = OPCODE_INR_C;

	sap_state->ram[0x000A] = OPCODE_HLT;

	// Data Section:
	sap_state->ram[0x0015] = 0x60;
	*/

	// Load Value in 0x3344 into A
	sap_state->ram[0x0000] = OPCODE_STA;
        sap_state->ram[0x0001] = 0x44;
        sap_state->ram[0x0002] = 0x33;

	// Move byte 2 into B
	sap_state->ram[0x0003] = OPCODE_MVI_B;
        sap_state->ram[0x0004] = 0x02;

	// A + B, then move B into C:
	sap_state->ram[0x0005] = OPCODE_ADD_B;
	sap_state->ram[0x0006] = OPCODE_MOV_C_B;

	// C - 1, then A = A - C
	sap_state->ram[0x0007] = OPCODE_DCR_C;
	sap_state->ram[0x0008] = OPCODE_SUB_C;

	sap_state->ram[0x0009] = OPCODE_JMP;
        sap_state->ram[0x000A] = 0x00;
        sap_state->ram[0x000B] = 0x05;

	sap_state->ram[0x0500] = OPCODE_INR_C;
	sap_state->ram[0x0501] = OPCODE_HLT;

	sap_state->ram[0x3344] = 0x03;

	printf("\nDumping memory before execution:\n");
	dump_sap_memory(sap_state);

	// Execute SAP:
	execute_sap(sap_state);

	// Dump State:
	dump_sap_state(sap_state);

	return 0;
}
