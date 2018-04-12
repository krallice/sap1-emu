#ifndef FLAG_H
#define FLAG_H

void set_zero_flag(sap_state_t *sap_state, int8_t *reg);
void set_sign_flag(sap_state_t *sap_state, int8_t *reg);
void set_flags(sap_state_t *sap_state, int8_t *reg);

#endif
