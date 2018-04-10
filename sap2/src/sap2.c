#include "sap2.h"

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

void set_zero_flag(sap_state_t *sap_state, int8_t *reg) {
	
	if (*reg != 0) {
		sap_state->flag_zero = 0;
	} else {
		sap_state->flag_zero = 1;
	}
}

void set_sign_flag(sap_state_t *sap_state, int8_t *reg) {
	
	if (*reg >= 0) {
		sap_state->flag_sign = 0;
	} else {
		sap_state->flag_sign = 1;
	}
}

void set_flags(sap_state_t *sap_state, int8_t *reg) {

	set_zero_flag(sap_state, reg);
	set_sign_flag(sap_state, reg);
}

void do_opcode_hlt(sap_state_t *sap_state) {
	printf("HLT: End of Program.\n");
}

void do_opcode_nop(sap_state_t *sap_state) {

	printf("NOP: Doing Nothing\n");
	sap_state->pc++;
}

void do_opcode_mvi(sap_state_t *sap_state, int8_t *dst_reg, char *dst_reg_name) {

	++sap_state->pc;
	printf("MVI: Move Value 0x%.2x into Register %s\n", sap_state->ram[sap_state->pc], dst_reg_name);
	*dst_reg = sap_state->ram[sap_state->pc];
	sap_state->pc++;
}

void do_opcode_lda(sap_state_t *sap_state) {

	// Todo: Smarter Bit Logic Here:
	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t load_address = (upper_byte << 8) | lower_byte;

	printf("LDA: Loading Address 0x%.4x (Value: 0x%.2x / %d) into Accumulator\n", load_address, sap_state->ram[load_address], sap_state->ram[load_address]);
	sap_state->a = sap_state->ram[load_address];
	sap_state->pc++;
}

void do_opcode_sta(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t store_address = (upper_byte << 8) | lower_byte;

	printf("STA: Storing Accumulator Value: (0x%.2x / %d) into Address 0x%.4x\n", sap_state->a, sap_state->a,
						store_address);
	sap_state->ram[store_address] = sap_state->a;
	sap_state->pc++;
}

void do_opcode_add(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	printf("ADD: Adding Value (0x%.2x / %d) from Register %s to Accumulator (0x%.2x / %d) with Result (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			sap_state->a, sap_state->a,
			(signed int)(sap_state->a + *src_reg), (signed int)(sap_state->a + *src_reg));
	sap_state->a += *src_reg;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_sub(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	printf("SUB: Subtracting Value (0x%.2x / %d) in Register %s from Accumulator (0x%.2x / %d) with Result (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			sap_state->a, sap_state->a,
			(signed int)(sap_state->a - *src_reg), (signed int)(sap_state->a - *src_reg));
	sap_state->a -= *src_reg;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_inr(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	printf("INR: Incrementing Value (0x%.2x / %d) in Register %s to Value (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			(signed int)(*src_reg + 1), (signed int)(*src_reg + 1));
	++*src_reg;
	set_flags(sap_state, src_reg);
	sap_state->pc++;
}

void do_opcode_dcr(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	printf("DCR: Decrementing Value (0x%.2x / %d) in Register %s to Value (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			(signed int)(*src_reg - 1), (signed int)(*src_reg - 1));
	--*src_reg;
	set_flags(sap_state, src_reg);
	sap_state->pc++;
}

void do_opcode_mov(sap_state_t *sap_state, int8_t *dst_reg, char *dst_reg_name, int8_t *src_reg, char *src_reg_name) {

	printf("MOV: Moving Value (0x%.2x / %d) from Register %s to Register %s\n",
			*src_reg, *src_reg, src_reg_name,
			dst_reg_name);
	*dst_reg = *src_reg;
	sap_state->pc++;
}

void do_opcode_jmp(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	printf("JMP: Setting PC to Address %.4x\n", jmp_address);
	sap_state->pc = jmp_address;
}

void do_opcode_jz(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	if ( sap_state->flag_zero == 1 ) {
		printf("JZ: Setting PC to Address %.4x\n", jmp_address);
		sap_state->pc = jmp_address;
	} else {
		printf("JZ: Zero Flag Not Set. No Jump taken\n");
		sap_state->pc++;
	}
}

void do_opcode_jnz(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	if ( sap_state->flag_zero == 1 ) {
		printf("JNZ: Zero Flag Not Set. No Jump taken\n");
		sap_state->pc++;
	} else {
		printf("JNZ: Setting PC to Address %.4x\n", jmp_address);
		sap_state->pc = jmp_address;
	}
}

void do_opcode_jm(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	if ( sap_state->flag_sign == 1 ) {
		printf("JM: Setting PC to Address %.4x\n", jmp_address);
		sap_state->pc = jmp_address;
	} else {
		printf("JM: Sign Flag not set. No jump taken\n");
		sap_state->pc++;
	}

}

void do_opcode_call(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t call_address = (upper_byte << 8) | lower_byte;

	// Increment PC and save at the highest 2 bytes of RAM:
	++sap_state->pc;
	sap_state->ram[0xFFFE] = sap_state->pc & 0xFF; // Lower Byte
	sap_state->ram[0xFFFF] = sap_state->pc >> 8; // Upper Byte

	// Jump:
	printf("CALL: Calling Routine at 0x%.4x\n", call_address);
	sap_state->pc = call_address;
}

void do_opcode_ret(sap_state_t *sap_state) {

	// Retrieve from RAM and Jump:
	uint8_t lower_byte = sap_state->ram[0xFFFE];
	uint8_t upper_byte = sap_state->ram[0xFFFF];
	uint16_t ret_address = (upper_byte << 8) | lower_byte;

	printf("RET: Returning to 0x%.4x\n", ret_address);
	sap_state->pc = ret_address;
}

void do_opcode_cma(sap_state_t *sap_state) {

	printf("CMA: Complemented the Accumulator from "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n", 
							BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(~sap_state->a)); 

	sap_state->a = ~sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	++sap_state->pc;
}

void do_opcode_ana(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	printf("ANA: ANDing Register %s ("BYTE_TO_BINARY_PATTERN") with Accumulator ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			src_reg_name, BYTE_TO_BINARY(*src_reg), BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(*src_reg & sap_state->a));

	sap_state->a = *src_reg & sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ani(sap_state_t *sap_state) {

	uint8_t next_byte = sap_state->ram[++sap_state->pc];

	printf("ANI: ANDing Accumulator ("BYTE_TO_BINARY_PATTERN") with next byte ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(next_byte), BYTE_TO_BINARY(sap_state->a & next_byte));

	sap_state->a = next_byte & sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ora(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	printf("ORA: ORing Register %s ("BYTE_TO_BINARY_PATTERN") with Accumulator ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			src_reg_name, BYTE_TO_BINARY(*src_reg), BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY((*src_reg | sap_state->a)));

	sap_state->a = *src_reg | sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ori(sap_state_t *sap_state) {

	uint8_t next_byte = sap_state->ram[++sap_state->pc];

	printf("ORI: ORing Accumulator ("BYTE_TO_BINARY_PATTERN") with next byte ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(next_byte), BYTE_TO_BINARY((sap_state->a | next_byte)));

	sap_state->a = next_byte | sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_xra(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	printf("XRA: XORing Register %s ("BYTE_TO_BINARY_PATTERN") with Accumulator ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			src_reg_name, BYTE_TO_BINARY(*src_reg), BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY((*src_reg ^ sap_state->a)));

	sap_state->a = *src_reg ^ sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_xri(sap_state_t *sap_state) {

	uint8_t next_byte = sap_state->ram[++sap_state->pc];

	printf("XRI: XORing Accumulator ("BYTE_TO_BINARY_PATTERN") with next byte ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(next_byte), BYTE_TO_BINARY((sap_state->a ^ next_byte)));

	sap_state->a = next_byte ^ sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ral(sap_state_t *sap_state) {

	int8_t rotated = ((uint8_t)sap_state->a << 1) | ((uint8_t)sap_state->a >> (8 - 1));
	printf("RAL: Rotating Accumulator Left "BYTE_TO_BINARY_PATTERN" -> "BYTE_TO_BINARY_PATTERN"\n",
		BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(rotated));
	sap_state->a = rotated;
	sap_state->pc++;
}

void do_opcode_rar(sap_state_t *sap_state) {

	int8_t rotated = ((uint8_t)sap_state->a >> 1) | ((uint8_t)sap_state->a << (8 - 1));
	printf("RAR: Rotating Accumulator Right "BYTE_TO_BINARY_PATTERN" -> "BYTE_TO_BINARY_PATTERN"\n",
		BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(rotated));
	sap_state->a = rotated;
	sap_state->pc++;
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
