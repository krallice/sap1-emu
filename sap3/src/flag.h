#ifndef FLAG_H
#define FLAG_H

#define FLAG_DEBUG 1

// Set Sign and Zero Flags:
void set_flags(sap_state_t *sap_state, int8_t *reg);
void set_zero_flag(sap_state_t *sap_state, int8_t *reg);
void set_sign_flag(sap_state_t *sap_state, int8_t *reg);

// Carry Flag Operations:
void set_carry_flag_add(sap_state_t *sap_state, int8_t *r1, int8_t *r2);
void set_carry_flag_sub(sap_state_t *sap_state, int8_t *r1, int8_t *r2);

uint8_t returnset_carry_flag(sap_state_t *sap_state);
void clear_carry_flag(sap_state_t *sap_state);
#endif
