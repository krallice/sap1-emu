#include "main.h"
#include "sap2.h"

void loadprog_lda_sta_5(sap_state_t *sap_state) {

	// Program: 2 + 3 = 5 the long way

	// Load Value in 0x3344 into A
	sap_state->ram[0x0000] = OPCODE_LDA;
        sap_state->ram[0x0001] = 0x44;
        sap_state->ram[0x0002] = 0x33;

	// Move byte 2 into B
	sap_state->ram[0x0003] = OPCODE_MVI_B;
        sap_state->ram[0x0004] = 0x02;

	// A + B, then move B into C:
	sap_state->ram[0x0005] = OPCODE_ADD_B;
	sap_state->ram[0x0006] = OPCODE_MOV_C_B;

	// Store Accumulator (5) into 0x0600:
	sap_state->ram[0x0007] = OPCODE_STA;
        sap_state->ram[0x0008] = 0x00;
        sap_state->ram[0x0009] = 0x06;

	// C - 1, then A = A - C
	sap_state->ram[0x000A] = OPCODE_DCR_C;
	sap_state->ram[0x000B] = OPCODE_SUB_C;

	// Jump to x0500:
	sap_state->ram[0x000C] = OPCODE_JMP;
        sap_state->ram[0x000D] = 0x00;
        sap_state->ram[0x000E] = 0x05;

	sap_state->ram[0x0500] = OPCODE_INR_C;

	// Move byte into B
	sap_state->ram[0x0501] = OPCODE_MVI_B;
        sap_state->ram[0x0502] = 0x02;

	// A + B:
	sap_state->ram[0x0503] = OPCODE_ADD_B;

	// Load our previously saved value into A:
	sap_state->ram[0x0504] = OPCODE_LDA;
        sap_state->ram[0x0505] = 0x00;
        sap_state->ram[0x0506] = 0x06;

	sap_state->ram[0x0507] = OPCODE_HLT;

	sap_state->ram[0x3344] = 0x03;
}

void loadprog_call_ret(sap_state_t *sap_state) {

	sap_state->ram[0x0000] = OPCODE_MVI_A;
        sap_state->ram[0x0001] = 0x07;

	// Call AddTwo() at x0700:
	sap_state->ram[0x0002] = OPCODE_CALL;
        sap_state->ram[0x0003] = 0x00;
        sap_state->ram[0x0004] = 0x07;

	// Return Here:
	sap_state->ram[0x0005] = OPCODE_HLT;

	// AddTwo() function:
	sap_state->ram[0x0700] = OPCODE_MVI_C;
        sap_state->ram[0x0701] = 0x02;
        sap_state->ram[0x0702] = OPCODE_ADD_C;
	sap_state->ram[0x0703] = OPCODE_RET;

}

void loadprog_jnz_test(sap_state_t *sap_state) {

	// Iterator C = 5:
	sap_state->ram[0x0000] = OPCODE_MVI_C;
        sap_state->ram[0x0001] = 0x05;

	// Multiplier B = 7:
	sap_state->ram[0x0002] = OPCODE_MVI_B;
        sap_state->ram[0x0003] = 0x07;

	// Product A = 0:
	sap_state->ram[0x0004] = OPCODE_MVI_A;
        sap_state->ram[0x0005] = 0x00;

	// while ( C != 0 )
	// 	A = A + B
	//	C--
	sap_state->ram[0x0006] = OPCODE_ADD_B;
	sap_state->ram[0x0007] = OPCODE_DCR_C;
	sap_state->ram[0x0008] = OPCODE_JNZ;
	sap_state->ram[0x0009] = 0x06;
	sap_state->ram[0x000A] = 0x00;

	sap_state->ram[0x000B] = OPCODE_HLT;
}

void loadprog_sign_test(sap_state_t *sap_state) {

	sap_state->ram[0x0001] = OPCODE_LDA;
        sap_state->ram[0x0002] = 0x00;
        sap_state->ram[0x0003] = 0x06;
	
	sap_state->ram[0x0004] = OPCODE_MOV_B_A;

	sap_state->ram[0x0005] = OPCODE_JMP;
        sap_state->ram[0x0006] = 0x05;
        sap_state->ram[0x0007] = 0x03;

	// Decrement until our sign bit is set:
        sap_state->ram[0x0305] = OPCODE_DCR_B;
        sap_state->ram[0x0306] = OPCODE_DCR_B;
        sap_state->ram[0x0307] = OPCODE_DCR_B;
        sap_state->ram[0x0308] = OPCODE_DCR_B;
        sap_state->ram[0x0309] = OPCODE_DCR_B;

	// Sign bit should be unset:
	//sap_state->ram[0x030A] = OPCODE_MOV_C_A;
        //sap_state->ram[0x030B] = OPCODE_INR_C;

	sap_state->ram[0x030C] = OPCODE_HLT;

        sap_state->ram[0x0600] = 0x03;
}

void loadprog_zero_test(sap_state_t *sap_state) {

	sap_state->ram[0x0001] = OPCODE_LDA;
        sap_state->ram[0x0002] = 0x00;
        sap_state->ram[0x0003] = 0x06;

	sap_state->ram[0x0004] = OPCODE_MOV_B_A;

	sap_state->ram[0x0005] = OPCODE_JMP;
        sap_state->ram[0x0006] = 0x05;
        sap_state->ram[0x0007] = 0x03;

        sap_state->ram[0x0305] = OPCODE_DCR_B;
        sap_state->ram[0x0306] = OPCODE_DCR_B;
        sap_state->ram[0x0307] = OPCODE_DCR_B; // Zero Flag should be set

	sap_state->ram[0x0308] = OPCODE_HLT;

        sap_state->ram[0x0600] = 0x03;
}

int main(void) {

	// Init our SAP Microcontroller:
	sap_state_t *sap_state = init_sap_state();
	if (sap_state == NULL) {
		return 1;
	}

	//loadprog_lda_sta_5(sap_state);
	//loadprog_call_ret(sap_state);
	//loadprog_jnz_test(sap_state);
	loadprog_sign_test(sap_state);
	//loadprog_zero_test(sap_state);

	printf("\nDumping memory before execution:\n");
	dump_sap_memory(sap_state);

	// Execute SAP:
	execute_sap(sap_state);

	// Dump State:
	dump_sap_state(sap_state);

	return 0;
}
