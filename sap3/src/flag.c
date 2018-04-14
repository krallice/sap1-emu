#include "sap.h"
#include "flag.h"
#include <limits.h>

void set_carry_flag_add(sap_state_t *sap_state, int8_t *r1, int8_t *r2) {
	#ifdef FLAG_DEBUG
	printf("\tCarry Flag Test: Is %d > (%d - %d)\n", (uint8_t)*r1, UCHAR_MAX, (uint8_t)*r2);
	#endif
	sap_state->flag_carry = (uint8_t)*r1 > UCHAR_MAX - (uint8_t)*r2;
}

void set_carry_flag_sub(sap_state_t *sap_state, int8_t *r1, int8_t *r2) {
	#ifdef FLAG_DEBUG
	printf("\tCarry Flag Test: Is %d < %d\n", (uint8_t)*r1, (uint8_t)*r2);
	#endif
	sap_state->flag_carry = (uint8_t)*r1 < (uint8_t)*r2;
}

void clear_carry_flag(sap_state_t *sap_state) {
	sap_state->flag_carry = 0;
}

void reset_carry_flag(sap_state_t *sap_state) {

	if ( sap_state->flag_carry == 1 ) {
		#ifdef FLAG_DEBUG
		printf("\tFlipping Carry Flag back to Zero\n");
		#endif
		sap_state->flag_carry = 0;
	}
}

void set_zero_flag(sap_state_t *sap_state, int8_t *reg) {
	
	if (*reg != 0) {
		sap_state->flag_zero = 0;
		return;
	}
	sap_state->flag_zero = 1;
}

void set_sign_flag(sap_state_t *sap_state, int8_t *reg) {
	
	if (*reg >= 0) {
		sap_state->flag_sign = 0;
		return;
	} 
	sap_state->flag_sign = 1;
}

void set_flags(sap_state_t *sap_state, int8_t *reg) {

	set_zero_flag(sap_state, reg);
	set_sign_flag(sap_state, reg);
}
