#include "sap.h"
#include "flag.h"
#include "opcode.h"

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

void do_opcode_hlt(sap_state_t *sap_state) {
	#ifdef OPCODE_DEBUG
	printf("HLT: End of Program.\n");
	#endif
}

void do_opcode_out(sap_state_t *sap_state) {
	#ifdef OPCODE_DEBUG
	printf("OUT: Dumping Processor State:\n\n");
	dump_sap_state(sap_state);
	printf("\n");
	#endif
	sap_state->pc++;
}

void do_opcode_nop(sap_state_t *sap_state) {

	#ifdef OPCODE_DEBUG
	printf("NOP: Doing Nothing\n");
	#endif
	sap_state->pc++;
}

void do_opcode_mvi(sap_state_t *sap_state, int8_t *dst_reg, char *dst_reg_name) {

	++sap_state->pc;
	#ifdef OPCODE_DEBUG
	printf("MVI: Move Value 0x%.2x into Register %s\n", sap_state->ram[sap_state->pc], dst_reg_name);
	#endif
	*dst_reg = sap_state->ram[sap_state->pc];
	sap_state->pc++;
}

void do_opcode_sta(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t store_address = (upper_byte << 8) | lower_byte;

	#ifdef OPCODE_DEBUG
	printf("STA: Storing Accumulator Value: (0x%.2x / %d) into Address 0x%.4x\n", sap_state->a, sap_state->a,
						store_address);
	#endif
	sap_state->ram[store_address] = sap_state->a;
	sap_state->pc++;
}

void do_opcode_lda(sap_state_t *sap_state) {

	// Todo: Smarter Bit Logic Here:
	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t load_address = (upper_byte << 8) | lower_byte;

	#ifdef OPCODE_DEBUG
	printf("LDA: Loading Address 0x%.4x (Value: 0x%.2x / %d) into Accumulator\n", load_address, sap_state->ram[load_address], sap_state->ram[load_address]);
	#endif
	sap_state->a = sap_state->ram[load_address];
	sap_state->pc++;
}

void do_opcode_add(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("ADD: Adding Value (0x%.2x / %d) from Register %s to Accumulator (0x%.2x / %d) with Result (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			sap_state->a, sap_state->a,
			(signed int)(sap_state->a + *src_reg), (signed int)(sap_state->a + *src_reg));
	#endif
	set_carry_flag_add(sap_state, &(sap_state->a), src_reg);
	sap_state->a += *src_reg;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_adc(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("ADC: Adding Value (0x%.2x / %d) from Register %s with Carry Flag %d to Accumulator (0x%.2x / %d) with Result (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name, sap_state->flag_carry,
			sap_state->a, sap_state->a,
			(signed int)(sap_state->a + *src_reg), (signed int)(sap_state->a + *src_reg + sap_state->flag_carry));
	#endif
	sap_state->a += (*src_reg + sap_state->flag_carry);
	//set_carry_flag_add(sap_state, &(sap_state->a), src_reg);
	reset_carry_flag(sap_state);
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_sub(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("SUB: Subtracting Value (0x%.2x / %d) in Register %s from Accumulator (0x%.2x / %d) with Result (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			sap_state->a, sap_state->a,
			(signed int)(sap_state->a - *src_reg), (signed int)(sap_state->a - *src_reg));
	#endif
	set_carry_flag_sub(sap_state, &(sap_state->a), src_reg);
	sap_state->a -= *src_reg;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_sbb(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("SBB: Subtracting Value (0x%.2x / %d) in Register %s with Carry Flag %d from Accumulator (0x%.2x / %d) with Result (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name, sap_state->flag_carry,
			sap_state->a, sap_state->a,
			(signed int)(sap_state->a - *src_reg), (signed int)(sap_state->a - *src_reg));
	#endif
	sap_state->a -= (*src_reg + sap_state->flag_carry);
	//set_carry_flag_sub(sap_state, &(sap_state->a), src_reg);
	reset_carry_flag(sap_state);
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_inr(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("INR: Incrementing Value (0x%.2x / %d) in Register %s to Value (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			(signed int)(*src_reg + 1), (signed int)(*src_reg + 1));
	#endif
	++*src_reg;
	set_flags(sap_state, src_reg);
	sap_state->pc++;
}

void do_opcode_dcr(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("DCR: Decrementing Value (0x%.2x / %d) in Register %s to Value (0x%.2x / %d)\n",
			*src_reg, *src_reg, src_reg_name,
			(signed int)(*src_reg - 1), (signed int)(*src_reg - 1));
	#endif
	--*src_reg;
	set_flags(sap_state, src_reg);
	sap_state->pc++;
}

void do_opcode_mov(sap_state_t *sap_state, int8_t *dst_reg, char *dst_reg_name, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("MOV: Moving Value (0x%.2x / %d) from Register %s to Register %s\n",
			*src_reg, *src_reg, src_reg_name,
			dst_reg_name);
	#endif
	*dst_reg = *src_reg;
	sap_state->pc++;
}

void do_opcode_jmp(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	#ifdef OPCODE_DEBUG
	printf("JMP: Setting PC to Address %.4x\n", jmp_address);
	#endif
	sap_state->pc = jmp_address;
}

void do_opcode_jz(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	if ( sap_state->flag_zero == 1 ) {
		#ifdef OPCODE_DEBUG
		printf("JZ: Setting PC to Address %.4x\n", jmp_address);
		#endif
		sap_state->pc = jmp_address;
	} else {
		#ifdef OPCODE_DEBUG
		printf("JZ: Zero Flag Not Set. No Jump taken\n");
		#endif
		sap_state->pc++;
	}
}

void do_opcode_jnz(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	if ( sap_state->flag_zero == 1 ) {
		#ifdef OPCODE_DEBUG
		printf("JNZ: Zero Flag Not Set. No Jump taken\n");
		#endif
		sap_state->pc++;
	} else {
		#ifdef OPCODE_DEBUG
		printf("JNZ: Setting PC to Address %.4x\n", jmp_address);
		#endif
		sap_state->pc = jmp_address;
	}
}

void do_opcode_jm(sap_state_t *sap_state) {

	uint8_t lower_byte = sap_state->ram[++sap_state->pc];
	uint8_t upper_byte = sap_state->ram[++sap_state->pc];
	uint16_t jmp_address = (upper_byte << 8) | lower_byte;

	if ( sap_state->flag_sign == 1 ) {
		#ifdef OPCODE_DEBUG
		printf("JM: Setting PC to Address %.4x\n", jmp_address);
		#endif
		sap_state->pc = jmp_address;
	} else {
		#ifdef OPCODE_DEBUG
		printf("JM: Sign Flag not set. No jump taken\n");
		#endif
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
	#ifdef OPCODE_DEBUG
	printf("CALL: Calling Routine at 0x%.4x\n", call_address);
	#endif
	sap_state->pc = call_address;
}

void do_opcode_ret(sap_state_t *sap_state) {

	// Retrieve from RAM and Jump:
	uint8_t lower_byte = sap_state->ram[0xFFFE];
	uint8_t upper_byte = sap_state->ram[0xFFFF];
	uint16_t ret_address = (upper_byte << 8) | lower_byte;

	#ifdef OPCODE_DEBUG
	printf("RET: Returning to 0x%.4x\n", ret_address);
	#endif
	sap_state->pc = ret_address;
}

void do_opcode_cma(sap_state_t *sap_state) {

	#ifdef OPCODE_DEBUG
	printf("CMA: Complemented the Accumulator from "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n", 
							BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(~sap_state->a)); 
	#endif
	sap_state->a = ~sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	++sap_state->pc;
}

void do_opcode_ana(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("ANA: ANDing Register %s ("BYTE_TO_BINARY_PATTERN") with Accumulator ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			src_reg_name, BYTE_TO_BINARY(*src_reg), BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(*src_reg & sap_state->a));
	#endif
	sap_state->a = *src_reg & sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ani(sap_state_t *sap_state) {

	uint8_t next_byte = sap_state->ram[++sap_state->pc];

	#ifdef OPCODE_DEBUG
	printf("ANI: ANDing Accumulator ("BYTE_TO_BINARY_PATTERN") with next byte ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(next_byte), BYTE_TO_BINARY(sap_state->a & next_byte));
	#endif
	sap_state->a = next_byte & sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ora(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("ORA: ORing Register %s ("BYTE_TO_BINARY_PATTERN") with Accumulator ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			src_reg_name, BYTE_TO_BINARY(*src_reg), BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY((*src_reg | sap_state->a)));
	#endif
	sap_state->a = *src_reg | sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ori(sap_state_t *sap_state) {

	uint8_t next_byte = sap_state->ram[++sap_state->pc];

	#ifdef OPCODE_DEBUG
	printf("ORI: ORing Accumulator ("BYTE_TO_BINARY_PATTERN") with next byte ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(next_byte), BYTE_TO_BINARY((sap_state->a | next_byte)));
	#endif
	sap_state->a = next_byte | sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_xra(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name) {

	#ifdef OPCODE_DEBUG
	printf("XRA: XORing Register %s ("BYTE_TO_BINARY_PATTERN") with Accumulator ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			src_reg_name, BYTE_TO_BINARY(*src_reg), BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY((*src_reg ^ sap_state->a)));
	#endif
	sap_state->a = *src_reg ^ sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_xri(sap_state_t *sap_state) {

	uint8_t next_byte = sap_state->ram[++sap_state->pc];

	#ifdef OPCODE_DEBUG
	printf("XRI: XORing Accumulator ("BYTE_TO_BINARY_PATTERN") with next byte ("BYTE_TO_BINARY_PATTERN") with Result "BYTE_TO_BINARY_PATTERN"\n",
			BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(next_byte), BYTE_TO_BINARY((sap_state->a ^ next_byte)));
	#endif
	sap_state->a = next_byte ^ sap_state->a;
	set_flags(sap_state, &(sap_state->a));
	sap_state->pc++;
}

void do_opcode_ral(sap_state_t *sap_state) {

	int8_t rotated = ((uint8_t)sap_state->a << 1) | ((uint8_t)sap_state->a >> (8 - 1));
	#ifdef OPCODE_DEBUG
	printf("RAL: Rotating Accumulator Left "BYTE_TO_BINARY_PATTERN" -> "BYTE_TO_BINARY_PATTERN"\n",
		BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(rotated));
	#endif
	sap_state->a = rotated;
	sap_state->pc++;
}

void do_opcode_rar(sap_state_t *sap_state) {

	int8_t rotated = ((uint8_t)sap_state->a >> 1) | ((uint8_t)sap_state->a << (8 - 1));
	#ifdef OPCODE_DEBUG
	printf("RAR: Rotating Accumulator Right "BYTE_TO_BINARY_PATTERN" -> "BYTE_TO_BINARY_PATTERN"\n",
		BYTE_TO_BINARY(sap_state->a), BYTE_TO_BINARY(rotated));
	#endif
	sap_state->a = rotated;
	sap_state->pc++;
}
