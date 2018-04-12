#include "sap.h"
#include "flag.h"

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
