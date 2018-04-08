#include "main.h"
#include "sap2.h"

int main(void) {

	// Init our SAP Microcontroller:
	sap_state_t *sap_state = init_sap_state();
	if (sap_state == NULL) {
		return 1;
	}

	// Load Value in 0x3344 into A
	sap_state->ram[0x0000] = OPCODE_LDA;
        sap_state->ram[0x0001] = 0x44;
        sap_state->ram[0x0002] = 0x33;

	// Move byte 2 into B
	sap_state->ram[0x0003] = OPCODE_MVI_B;
        sap_state->ram[0x0004] = 0x02;

	// A + B, then move B into C:
	sap_state->ram[0x0005] = OPCODE_ADD_B;
	sap_state->ram[0x0006] = OPCODE_MOV_C_B;

	// Store Accumulator (5) into 0x0600:
	sap_state->ram[0x0007] = OPCODE_STA;
        sap_state->ram[0x0008] = 0x00;
        sap_state->ram[0x0009] = 0x06;

	// C - 1, then A = A - C
	sap_state->ram[0x000A] = OPCODE_DCR_C;
	sap_state->ram[0x000B] = OPCODE_SUB_C;

	// Jump to x0500:
	sap_state->ram[0x000C] = OPCODE_JMP;
        sap_state->ram[0x000D] = 0x00;
        sap_state->ram[0x000E] = 0x05;

	sap_state->ram[0x0500] = OPCODE_INR_C;

	// Move byte into B
	sap_state->ram[0x0501] = OPCODE_MVI_B;
        sap_state->ram[0x0502] = 0x02;

	// A + B:
	sap_state->ram[0x0503] = OPCODE_ADD_B;

	// Load our previously saved value into A:
	sap_state->ram[0x0504] = OPCODE_LDA;
        sap_state->ram[0x0505] = 0x00;
        sap_state->ram[0x0506] = 0x06;

	sap_state->ram[0x0507] = OPCODE_HLT;

	sap_state->ram[0x3344] = 0x03;

	printf("\nDumping memory before execution:\n");
	dump_sap_memory(sap_state);

	// Execute SAP:
	execute_sap(sap_state);

	// Dump State:
	dump_sap_state(sap_state);

	return 0;
}
