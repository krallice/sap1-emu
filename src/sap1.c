#include "sap1.h"

typedef struct sap1_state_t {
        uint8_t a;
        uint8_t b;
} sap_state_t;

sap_state_t *init_sap_state(void) {

	sap_state_t *sap_state = (sap_state_t*)malloc(sizeof(sap_state_t));
	return sap_state;
	
}
