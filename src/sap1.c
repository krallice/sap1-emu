#include "sap1.h"

// Initialise our State
sap_state_t *init_sap_state(void) {

	// Initialise our main struct:
	sap_state_t *sap_state = (sap_state_t*)malloc(sizeof(sap_state_t));
	if ( sap_state == NULL ) {
		goto errorout;
	}
	memset(sap_state, 0, sizeof(sap_state_t));

	// Initialise RAM:
	sap_state->ram_size = SAP_RAM_SIZE;
	sap_state->ram = (uint8_t*)malloc(sizeof(uint8_t)*sap_state->ram_size);
	if ( sap_state->ram == NULL ) {
		goto errorout;
	}
	memset(sap_state->ram, 0, sizeof(uint8_t)*sap_state->ram_size);

	return sap_state;

	errorout:
		#if SAP_DEBUG
		printf("Unable to initialise SAP/Memory Struct\n");
		#endif
		return NULL;
}

void dump_sap_memory(sap_state_t *sap_state) {
	for (int i = 0; i < sap_state->ram_size; i++) {
		printf("Address: 0x%.1x\tData: 0x%.2x\n", i, sap_state->ram[i]);
	}		
}

void do_opcode_hlt(sap_state_t *sap_state) {
	printf("HALT Reached - TODO: Cleanup Code\n");
}

void execute_sap(sap_state_t *sap_state) {

	#if SAP_DEBUG
	printf("Starting Main Execution Loop\n");
	#endif

	while(1) {

		if ((sap_state->ram[sap_state->pc] >> 4) & (OPCODE_HLT)) {
			do_opcode_hlt(sap_state);
			break;
		}

		// Increment Program Counter:
		sap_state->pc++;

		usleep(1000);
		#if SAP_DEBUG
		printf("End of Cycle\n");
		#endif
	}
}
