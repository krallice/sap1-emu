#include "main.h"
#include "sap1.h"

int main(void) {

	// Init our SAP Microcontroller:
	sap_state_t *sap_state = init_sap_state();
	if (sap_state == NULL) {
		return 1;
	}

	// Program Memory:
	sap_state->ram[0] = OPCODE_HLT << 4;

	printf("Dumping memory before execution:\n");
	dump_sap_memory(sap_state);

	// Execute SAP:
	execute_sap(sap_state);

	return 0;
}
