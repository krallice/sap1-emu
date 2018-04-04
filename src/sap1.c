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
	memset(sap_state->ram, 0xEE, sizeof(uint8_t)*sap_state->ram_size);

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

void dump_sap_state(sap_state_t *sap_state) {

	printf("\nDumping SAP State ::\n");
	printf("Register A (Accumulator): 0x%.2x\n", sap_state->a);
	printf("Register B:\t\t0x%.2x\n", sap_state->b);
	printf("Program Counter:\t0x%.2x\n", sap_state->pc);
}

void do_opcode_hlt(sap_state_t *sap_state) {
	printf("HLT: End of Program.\n");
}

void do_opcode_add(sap_state_t *sap_state) {

	uint8_t ref_address = sap_state->ram[sap_state->pc] & OPERAND_MASK;
	printf("ADD: Add Address 0x%.1x (Value: 0x%.2x / %d) to Accumulator (Old: 0x%.2x New: 0x%.2x)\n", ref_address, sap_state->ram[ref_address], 
													  sap_state->ram[ref_address], sap_state->a,
													  sap_state->a + sap_state->ram[ref_address]);
	sap_state->a += sap_state->ram[ref_address];
}

void do_opcode_sub(sap_state_t *sap_state) {

	uint8_t ref_address = sap_state->ram[sap_state->pc] & OPERAND_MASK;
	printf("SUB: SUB Address 0x%.1x (Value: 0x%.2x / %d) from Accumulator (Old: 0x%.2x New: 0x%.2x)\n", ref_address, sap_state->ram[ref_address], 
													  sap_state->ram[ref_address], sap_state->a,
													  sap_state->a - sap_state->ram[ref_address]);
	sap_state->a -= sap_state->ram[ref_address];
}

void do_opcode_lda(sap_state_t *sap_state) {

	uint8_t ref_address = sap_state->ram[sap_state->pc] & OPERAND_MASK;
	printf("LDA: Loading Address 0x%.1x (Value: 0x%.2x / %d) into Accumulator\n", ref_address, sap_state->ram[ref_address], sap_state->ram[ref_address]);
	sap_state->a = sap_state->ram[ref_address];
}

void execute_sap(sap_state_t *sap_state) {

	#if SAP_DEBUG
	printf("\nStarting Main Execution Loop\n");
	#endif

	while(1) {

		uint8_t opcode = (sap_state->ram[sap_state->pc] & OPCODE_MASK) >> 4;
		printf("Instruction: 0x%.1x\n", opcode);

		switch (opcode) {
			case OPCODE_HLT:
				do_opcode_hlt(sap_state);
				return;

			case OPCODE_LDA:
				do_opcode_lda(sap_state);
				break;

			case OPCODE_ADD:
				do_opcode_add(sap_state);
				break;

			case OPCODE_SUB:
				do_opcode_sub(sap_state);
				break;

			default:
				printf("Unknown Opcode: 0x%.1x. Ignoring\n", sap_state->ram[sap_state->pc] >> 4);
				break;
		}

		// Increment Program Counter:
		sap_state->pc++;

		usleep(1000);
	}
}
