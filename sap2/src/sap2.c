#include "sap2.h"

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
	memset(sap_state->ram, OPCODE_NOP, sizeof(uint8_t)*sap_state->ram_size);

	return sap_state;

	errorout:
		#if SAP_DEBUG
		printf("Unable to initialise SAP/Memory Struct\n");
		#endif
		return NULL;
}

void dump_sap_memory(sap_state_t *sap_state) {
	
	// Dump the first 0x16 Words Only:
	for (int i = 0; i < 0x16; i++) {
		printf("Address: 0x%.4x\tData: 0x%.2x\n", i, sap_state->ram[i]);
	}		
}

void dump_sap_state(sap_state_t *sap_state) {

	printf("\nDumping SAP State ::\n");
	printf("Register A (Accumulator): 0x%.2x\n", sap_state->a);
	printf("Register B:\t\t0x%.2x\n", sap_state->b);
	printf("Register C:\t\t0x%.2x\n", sap_state->c);
	printf("Register TMP:\t\t0x%.2x\n", sap_state->tmp);
	printf("Zero Flag:\t\t%d\n", sap_state->flag_zero);
	printf("Sign Flag:\t\t%d\n", sap_state->flag_sign);
	printf("Program Counter:\t0x%.2x\n", sap_state->pc);
}

void do_opcode_hlt(sap_state_t *sap_state) {
	printf("HLT: End of Program.\n");
}

void do_opcode_nop(sap_state_t *sap_state) {

	printf("NOP: Doing Nothing\n");
}

void do_opcode_mvi(sap_state_t *sap_state, uint8_t *dst_reg, char *dst_reg_name) {

	++sap_state->pc;
	printf("MVI: Move Value 0x%.2x into Register %s\n", sap_state->ram[sap_state->pc], dst_reg_name);
	*dst_reg = sap_state->ram[sap_state->pc];
}

void do_opcode_sta(sap_state_t *sap_state) {

	// Todo: Smarter Bit Logic Here:
	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t load_address = (upper_byte << 8) | lower_byte;

	printf("STA: Loading Address 0x%.4x (Value: 0x%.2x / %d) into Accumulator\n", load_address, sap_state->ram[load_address], sap_state->ram[load_address]);
	sap_state->a = sap_state->ram[load_address];
}

void do_opcode_add(sap_state_t *sap_state, uint8_t *src_reg, char *src_reg_name) {

	printf("ADD: Adding Value (0x%.2x / %d) from Register %s to Accumulator (0x%.2x / %d) with Result (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			sap_state->a, sap_state->a,
			(int)(sap_state->a + *src_reg), (int)(sap_state->a + *src_reg));
	sap_state->a += *src_reg;
}

void do_opcode_inr(sap_state_t *sap_state, uint8_t *src_reg, char *src_reg_name) {

	printf("INR: Incrementing Value (0x%.2x / %d) in Register %s to Value (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			(int)(*src_reg + 1), (int)(*src_reg + 1));
	++*src_reg;
}

void execute_sap(sap_state_t *sap_state) {

	#if SAP_DEBUG
	printf("\nStarting Main Execution Loop\n");
	#endif

	while(1) {

		uint8_t opcode = (sap_state->ram[sap_state->pc]);
		printf("Instruction: 0x%.2x ", opcode);

		switch (opcode) {
			case OPCODE_HLT:
				do_opcode_hlt(sap_state);
				return;

			case OPCODE_NOP:
				do_opcode_nop(sap_state);
				break;

			case OPCODE_MVI_A:
				do_opcode_mvi(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_MVI_B:
				do_opcode_mvi(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_MVI_C:
				do_opcode_mvi(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_STA:
				do_opcode_sta(sap_state);
				break;

			case OPCODE_ADD_B:
				do_opcode_add(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_ADD_C:
				do_opcode_add(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_INR_A:
				do_opcode_inr(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_INR_B:
				do_opcode_inr(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_INR_C:
				do_opcode_inr(sap_state, &(sap_state->c), "C");
				break;

			default:
				printf("Unknown Opcode: 0x%.2x. Ignoring\n", sap_state->ram[sap_state->pc]);
				break;
		}

		// Increment Program Counter:
		sap_state->pc++;

		usleep(1000);
	}
}
