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


	printf("Register A (Accumulator): 0x%.2x\n", (uint8_t)sap_state->a);

	printf("Register B:\t\t0x%.2x  (Register BC:0x%.2x%.2x)\n", (uint8_t)sap_state->b, (uint8_t)sap_state->b, (uint8_t)sap_state->c);
	printf("Register C:\t\t0x%.2x\n", (uint8_t)sap_state->c);
	printf("Register D:\t\t0x%.2x  (Register DE:0x%.2x%.2x)\n", (uint8_t)sap_state->d, (uint8_t)sap_state->d, (uint8_t)sap_state->e);
	printf("Register E:\t\t0x%.2x\n", (uint8_t)sap_state->e);
	printf("Register H:\t\t0x%.2x  (Register HL:0x%.2x%.2x)\n", (uint8_t)sap_state->h, (uint8_t)sap_state->h, (uint8_t)sap_state->l);
	printf("Register L:\t\t0x%.2x\n", (uint8_t)sap_state->l);

	printf("Zero Flag:\t\t%d\n", sap_state->flag_zero);
	printf("Sign Flag:\t\t%d\n", sap_state->flag_sign);
	printf("Carry Flag:\t\t%d\n", sap_state->flag_carry);

	printf("Program Counter:\t0x%.4x\n", sap_state->pc);
	printf("Stack Pointer:\t\t0x%.4x\n", sap_state->sp);
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

			case OPCODE_OUT:
				do_opcode_out(sap_state);
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

			case OPCODE_MVI_D:
				do_opcode_mvi(sap_state, &(sap_state->d), "D");
				break;

			case OPCODE_MVI_E:
				do_opcode_mvi(sap_state, &(sap_state->e), "E");
				break;

			case OPCODE_MVI_H:
				do_opcode_mvi(sap_state, &(sap_state->h), "H");
				break;

			case OPCODE_MVI_L:
				do_opcode_mvi(sap_state, &(sap_state->l), "L");
				break;

			case OPCODE_LDA:
				do_opcode_lda(sap_state);
				break;

			// Store:
			case OPCODE_STA:
				do_opcode_sta(sap_state);
				break;





			// Adds:
			case OPCODE_ADD_A:
				do_opcode_add(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_ADD_B:
				do_opcode_add(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_ADD_C:
				do_opcode_add(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_ADD_D:
				do_opcode_add(sap_state, &(sap_state->d), "D");
				break;

			case OPCODE_ADD_E:
				do_opcode_add(sap_state, &(sap_state->e), "E");
				break;

			case OPCODE_ADD_H:
				do_opcode_add(sap_state, &(sap_state->h), "H");
				break;

			case OPCODE_ADD_L:
				do_opcode_add(sap_state, &(sap_state->l), "L");
				break;

			case OPCODE_ADC_A:
				do_opcode_adc(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_ADC_B:
				do_opcode_adc(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_ADC_C:
				do_opcode_adc(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_ADC_D:
				do_opcode_adc(sap_state, &(sap_state->d), "D");
				break;

			case OPCODE_ADC_E:
				do_opcode_adc(sap_state, &(sap_state->e), "E");
				break;

			case OPCODE_ADC_H:
				do_opcode_adc(sap_state, &(sap_state->h), "H");
				break;

			case OPCODE_ADC_L:
				do_opcode_adc(sap_state, &(sap_state->l), "L");
				break;



			// Subtraction:
			case OPCODE_SUB_A:
				do_opcode_sub(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_SUB_B:
				do_opcode_sub(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_SUB_C:
				do_opcode_sub(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_SUB_D:
				do_opcode_sub(sap_state, &(sap_state->d), "D");
				break;

			case OPCODE_SUB_E:
				do_opcode_sub(sap_state, &(sap_state->e), "E");
				break;

			case OPCODE_SUB_H:
				do_opcode_sub(sap_state, &(sap_state->h), "H");
				break;

			case OPCODE_SUB_L:
				do_opcode_sub(sap_state, &(sap_state->l), "L");
				break;

			case OPCODE_SBB_A:
				do_opcode_sbb(sap_state, &(sap_state->a), "A");
				break;

			case OPCODE_SBB_B:
				do_opcode_sbb(sap_state, &(sap_state->b), "B");
				break;

			case OPCODE_SBB_C:
				do_opcode_sbb(sap_state, &(sap_state->c), "C");
				break;

			case OPCODE_SBB_D:
				do_opcode_sbb(sap_state, &(sap_state->d), "D");
				break;

			case OPCODE_SBB_E:
				do_opcode_sbb(sap_state, &(sap_state->e), "E");
				break;

			case OPCODE_SBB_H:
				do_opcode_sbb(sap_state, &(sap_state->h), "H");
				break;

			case OPCODE_SBB_L:
				do_opcode_sbb(sap_state, &(sap_state->l), "L");
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

			case OPCODE_INR_D:
				do_opcode_inr(sap_state, &(sap_state->d), "D");
				break;

			case OPCODE_INR_E:
				do_opcode_inr(sap_state, &(sap_state->e), "E");
				break;

			case OPCODE_INR_H:
				do_opcode_inr(sap_state, &(sap_state->h), "H");
				break;

			case OPCODE_INR_L:
				do_opcode_inr(sap_state, &(sap_state->l), "L");
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

			case OPCODE_DCR_D:
				do_opcode_dcr(sap_state, &(sap_state->d), "D");
				break;

			case OPCODE_DCR_E:
				do_opcode_dcr(sap_state, &(sap_state->e), "E");
				break;

			case OPCODE_DCR_H:
				do_opcode_dcr(sap_state, &(sap_state->h), "H");
				break;

			case OPCODE_DCR_L:
				do_opcode_dcr(sap_state, &(sap_state->l), "L");
				break;








			// Moves:
			case OPCODE_MOV_A_B: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->b), "B");
				break;

			case OPCODE_MOV_A_C: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_A_D: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->d), "D");
				break;

			case OPCODE_MOV_A_E: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->e), "E");
				break;

			case OPCODE_MOV_A_H: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->h), "H");
				break;

			case OPCODE_MOV_A_L: 
				do_opcode_mov(sap_state, &(sap_state->a), "A", &(sap_state->l), "L");
				break;


			case OPCODE_MOV_B_A: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_B_C: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_B_D: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->d), "D");
				break;

			case OPCODE_MOV_B_E: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->e), "E");
				break;

			case OPCODE_MOV_B_H: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->h), "H");
				break;

			case OPCODE_MOV_B_L: 
				do_opcode_mov(sap_state, &(sap_state->b), "B", &(sap_state->l), "L");
				break;



			case OPCODE_MOV_C_A: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_C_B: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->b), "B");
				break;

			case OPCODE_MOV_C_D: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->d), "D");
				break;

			case OPCODE_MOV_C_E: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->e), "E");
				break;

			case OPCODE_MOV_C_H: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->h), "H");
				break;

			case OPCODE_MOV_C_L: 
				do_opcode_mov(sap_state, &(sap_state->c), "C", &(sap_state->l), "L");
				break;




			case OPCODE_MOV_D_A: 
				do_opcode_mov(sap_state, &(sap_state->d), "D", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_D_B: 
				do_opcode_mov(sap_state, &(sap_state->d), "D", &(sap_state->b), "B");
				break;

			case OPCODE_MOV_D_C: 
				do_opcode_mov(sap_state, &(sap_state->d), "D", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_D_E: 
				do_opcode_mov(sap_state, &(sap_state->d), "D", &(sap_state->e), "E");
				break;

			case OPCODE_MOV_D_H: 
				do_opcode_mov(sap_state, &(sap_state->d), "D", &(sap_state->h), "H");
				break;

			case OPCODE_MOV_D_L: 
				do_opcode_mov(sap_state, &(sap_state->d), "D", &(sap_state->l), "L");
				break;



			case OPCODE_MOV_E_A: 
				do_opcode_mov(sap_state, &(sap_state->e), "E", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_E_B: 
				do_opcode_mov(sap_state, &(sap_state->e), "E", &(sap_state->b), "B");
				break;

			case OPCODE_MOV_E_C: 
				do_opcode_mov(sap_state, &(sap_state->e), "E", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_E_D: 
				do_opcode_mov(sap_state, &(sap_state->e), "E", &(sap_state->d), "D");
				break;

			case OPCODE_MOV_E_H: 
				do_opcode_mov(sap_state, &(sap_state->e), "E", &(sap_state->h), "H");
				break;

			case OPCODE_MOV_E_L: 
				do_opcode_mov(sap_state, &(sap_state->e), "E", &(sap_state->l), "L");
				break;



			case OPCODE_MOV_H_A: 
				do_opcode_mov(sap_state, &(sap_state->h), "H", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_H_B: 
				do_opcode_mov(sap_state, &(sap_state->h), "H", &(sap_state->b), "B");
				break;

			case OPCODE_MOV_H_C: 
				do_opcode_mov(sap_state, &(sap_state->h), "H", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_H_D: 
				do_opcode_mov(sap_state, &(sap_state->h), "H", &(sap_state->d), "D");
				break;

			case OPCODE_MOV_H_E: 
				do_opcode_mov(sap_state, &(sap_state->h), "H", &(sap_state->e), "E");
				break;

			case OPCODE_MOV_H_L: 
				do_opcode_mov(sap_state, &(sap_state->h), "H", &(sap_state->l), "L");
				break;



			case OPCODE_MOV_L_A: 
				do_opcode_mov(sap_state, &(sap_state->l), "L", &(sap_state->a), "A");
				break;

			case OPCODE_MOV_L_B: 
				do_opcode_mov(sap_state, &(sap_state->l), "L", &(sap_state->b), "B");
				break;

			case OPCODE_MOV_L_C: 
				do_opcode_mov(sap_state, &(sap_state->l), "L", &(sap_state->c), "C");
				break;

			case OPCODE_MOV_L_D: 
				do_opcode_mov(sap_state, &(sap_state->l), "L", &(sap_state->d), "D");
				break;

			case OPCODE_MOV_L_E: 
				do_opcode_mov(sap_state, &(sap_state->l), "L", &(sap_state->e), "E");
				break;

			case OPCODE_MOV_L_H: 
				do_opcode_mov(sap_state, &(sap_state->l), "L", &(sap_state->h), "H");
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
