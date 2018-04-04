#include "main.h"
#include "sap1.h"

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
	sap_state->ram[0x0] = (OPCODE_LDA << 4) + 0xA;
	sap_state->ram[0x1] = (OPCODE_ADD << 4) + 0xB;
	sap_state->ram[0x2] = OPCODE_HLT << 4;

	// Data Section:
	sap_state->ram[0xA] = 0x50;
	sap_state->ram[0xB] = 0x60;

	printf("\nDumping memory before execution:\n");
	dump_sap_memory(sap_state);

	// Execute SAP:
	execute_sap(sap_state);

	// Dump State:
	dump_sap_state(sap_state);

	return 0;
}
