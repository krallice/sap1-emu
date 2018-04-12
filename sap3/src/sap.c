#include "sap.h"
#include "opcode.h"
#include "flag.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

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
	printf("Register A (Accumulator): 0x%.2x\n", (uint8_t)sap_state->a);
	printf("Register B:\t\t0x%.2x\n", (uint8_t)sap_state->b);
	printf("Register C:\t\t0x%.2x\n", (uint8_t)sap_state->c);
	printf("Zero Flag:\t\t%d\n", sap_state->flag_zero);
	printf("Sign Flag:\t\t%d\n", sap_state->flag_sign);
	printf("Program Counter:\t0x%.4x\n", sap_state->pc);
}


void execute_sap(sap_state_t *sap_state) {

	#if SAP_DEBUG
	printf("\nStarting Main Execution Loop\n");
	#endif

	while(1) {

		uint8_t opcode = (sap_state->ram[sap_state->pc]);
		printf("Instruction: 0x%.2x ", opcode);

		switch (opcode) {
			// Misc:
			case OPCODE_HLT:
				do_opcode_hlt(sap_state);
				return;

			case OPCODE_NOP:
				do_opcode_nop(sap_state);
				break;

			// Jumps:
			case OPCODE_JMP:
				do_opcode_jmp(sap_state);
				break;

			case OPCODE_JZ:
				do_opcode_jz(sap_state);
				break;

			case OPCODE_JNZ:
				do_opcode_jnz(sap_state);
				break;

			case OPCODE_JM:
				do_opcode_jm(sap_state);
				break;

			case OPCODE_CALL:
				do_opcode_call(sap_state);
				break;

			case OPCODE_RET:
				do_opcode_ret(sap_state);
				break;

			// Loads:
			case OPCODE_MVI_A:
				do_opcode_mvi(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_MVI_B:
				do_opcode_mvi(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_MVI_C:
				do_opcode_mvi(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_LDA:
				do_opcode_lda(sap_state);
				break;

			// Store:
			case OPCODE_STA:
				do_opcode_sta(sap_state);
				break;

			// Adds:
			case OPCODE_ADD_B:
				do_opcode_add(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_ADD_C:
				do_opcode_add(sap_state, &(sap_state->c), "C");
				break;

			// Subtraction:
			case OPCODE_SUB_B:
				do_opcode_sub(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_SUB_C:
				do_opcode_sub(sap_state, &(sap_state->c), "C");
				break;

			// Increments:
			case OPCODE_INR_A:
				do_opcode_inr(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_INR_B:
				do_opcode_inr(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_INR_C:
				do_opcode_inr(sap_state, &(sap_state->c), "C");
				break;

			// Decrements:
			case OPCODE_DCR_A:
				do_opcode_dcr(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_DCR_B:
				do_opcode_dcr(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_DCR_C:
				do_opcode_dcr(sap_state, &(sap_state->c), "C");
				break;

			// Moves:
			case OPCODE_MOV_A_B: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->b), "B");
				break;

			case OPCODE_MOV_A_C: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_B_A: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_B_C: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_C_A: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_C_B: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->b), "B");
				break;

			// Logic:
			case OPCODE_CMA:
				do_opcode_cma(sap_state);
				break;

			case OPCODE_ANA_B:
				do_opcode_ana(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_ANA_C:
				do_opcode_ana(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_ANI:
				do_opcode_ani(sap_state);
				break;

			case OPCODE_ORA_B:
				do_opcode_ora(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_ORA_C:
				do_opcode_ora(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_ORI:
				do_opcode_ori(sap_state);
				break;

			case OPCODE_XRA_B:
				do_opcode_xra(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_XRA_C:
				do_opcode_xra(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_XRI:
				do_opcode_xri(sap_state);
				break;

			// Rotations:
			case OPCODE_RAR:
				do_opcode_rar(sap_state);
				break;

			case OPCODE_RAL:
				do_opcode_ral(sap_state);
				break;

			default:
				printf("Unknown Opcode: 0x%.2x. Ignoring\n", sap_state->ram[sap_state->pc]);
				break;
		}

		usleep(10000);
		//usleep(1000000);
	}
}
