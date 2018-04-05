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
	sap_state->ram[0x0000] = OPCODE_MVI_B;
	sap_state->ram[0x0001] = 0x20;

	sap_state->ram[0x0002] = OPCODE_MVI_C;
	sap_state->ram[0x0003] = 0x30;

	sap_state->ram[0x0004] = OPCODE_STA;
	sap_state->ram[0x0005] = 0x15;
	sap_state->ram[0x0006] = 0x00;

	sap_state->ram[0x0007] = OPCODE_ADD_B;

	sap_state->ram[0x0009] = OPCODE_HLT;

	// Data Section:
	sap_state->ram[0x0015] = 0x60;

	printf("\nDumping memory before execution:\n");
	dump_sap_memory(sap_state);

	// Execute SAP:
	execute_sap(sap_state);

	// Dump State:
	dump_sap_state(sap_state);

	return 0;
}
