#include "main.h"

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
	sap_state->ram[0x0002] = OPCODE_MVI_E;
        sap_state->ram[0x0003] = 0x03;

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

	sap_state->ram[0x000B] = OPCODE_MVI_L;
        sap_state->ram[0x000C] = 0x07;

	sap_state->ram[0x000D] = OPCODE_HLT;
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

void loadprog_cma_test(sap_state_t *sap_state) {

	// CMA Testing:
	sap_state->ram[0x0000] = OPCODE_MVI_A;
	sap_state->ram[0x0001] = 0x01;
	sap_state->ram[0x0002] = OPCODE_CMA;

	sap_state->ram[0x0003] = OPCODE_MVI_A;
	sap_state->ram[0x0004] = 0xE4;
	sap_state->ram[0x0005] = OPCODE_CMA;

	sap_state->ram[0x0006] = OPCODE_CMA;

	sap_state->ram[0x0007] = OPCODE_JNZ;
        sap_state->ram[0x0008] = 0x50;
        sap_state->ram[0x0009] = 0x00;

	// AND:
	sap_state->ram[0x0050] = OPCODE_MVI_B;
        sap_state->ram[0x0051] = 0xE4;
	sap_state->ram[0x0052] = OPCODE_ANA_B;

	sap_state->ram[0x0053] = OPCODE_CALL;
        sap_state->ram[0x0054] = 0x50;
        sap_state->ram[0x0055] = 0x70;

	// Return:
	// OR:
	sap_state->ram[0x0056] = OPCODE_ORI;
        sap_state->ram[0x0057] = 0x6F;

	sap_state->ram[0x0058] = OPCODE_ORA_B;

	sap_state->ram[0x0065] = OPCODE_HLT;

		// Func:
		sap_state->ram[0x7050] = OPCODE_MVI_C;
		sap_state->ram[0x7051] = 0xF2;
		sap_state->ram[0x7052] = OPCODE_ANA_C;

		sap_state->ram[0x7053] = OPCODE_MVI_C;
		sap_state->ram[0x7054] = 0xB7;

		// XOR:
		sap_state->ram[0x7055] = OPCODE_XRA_C;
		sap_state->ram[0x7056] = OPCODE_XRI;
		sap_state->ram[0x7057] = 0x70;

		sap_state->ram[0x7058] = OPCODE_RET;
}

void loadprog_rar_test(sap_state_t *sap_state) {

	sap_state->ram[0x0000] = OPCODE_MVI_A;
	sap_state->ram[0x0001] = 0x01;

	sap_state->ram[0x0002] = OPCODE_RAL;
	sap_state->ram[0x0003] = OPCODE_RAL;

	sap_state->ram[0x0004] = OPCODE_MVI_A;
	sap_state->ram[0x0005] = 0x40;

	sap_state->ram[0x0006] = OPCODE_RAL;
	sap_state->ram[0x0007] = OPCODE_RAL;

	sap_state->ram[0x0008] = OPCODE_MVI_A;
	sap_state->ram[0x0009] = 0x08;

	sap_state->ram[0x000A] = OPCODE_RAR;
	sap_state->ram[0x000B] = OPCODE_RAR;
	sap_state->ram[0x000C] = OPCODE_RAR;
	sap_state->ram[0x000D] = OPCODE_RAR;

	sap_state->ram[0x000F] = OPCODE_HLT;
}

void loadprog_fib(sap_state_t *sap_state) {

	sap_state->ram[0x0000] = OPCODE_MVI_B;
	sap_state->ram[0x0001] = 0x00;

	sap_state->ram[0x0000] = OPCODE_STA;
	sap_state->ram[0x0001] = 0x00;
	sap_state->ram[0x0002] = 0x0B;

	sap_state->ram[0x0002] = OPCODE_OUT;

	// .data:
	sap_state->ram[0x0B00] = 0x00;


}

void loadprog_carry_test(sap_state_t *sap_state) {

	sap_state->ram[0x0000] = OPCODE_MVI_A; // All Flags should be 0
	sap_state->ram[0x0001] = 0xFE;
	sap_state->ram[0x0002] = OPCODE_MVI_B;
	sap_state->ram[0x0003] = 0x02;
	sap_state->ram[0x0004] = OPCODE_ADD_B;
	sap_state->ram[0x0005] = OPCODE_OUT;

	sap_state->ram[0x0008] = OPCODE_MVI_B;
	sap_state->ram[0x0009] = 0x02;
	sap_state->ram[0x000A] = OPCODE_ADC_B;
	sap_state->ram[0x000B] = OPCODE_OUT;

	/*
	sap_state->ram[0x000C] = OPCODE_MVI_A; // Sign Flag should be 1
	sap_state->ram[0x000D] = 0xFE;
	sap_state->ram[0x000E] = OPCODE_MVI_B;
	sap_state->ram[0x000F] = 0x01;
	sap_state->ram[0x0010] = OPCODE_ADD_B;
	sap_state->ram[0x0011] = OPCODE_OUT;

	sap_state->ram[0x0012] = OPCODE_MVI_A; // Sign Flag should be 1
	sap_state->ram[0x0013] = 0xFE;
	sap_state->ram[0x0014] = OPCODE_MVI_B;
	sap_state->ram[0x0015] = 0x0A;
	sap_state->ram[0x0016] = OPCODE_ADD_B;
	sap_state->ram[0x0017] = OPCODE_OUT;
	*/

	/*
	sap_state->ram[0x0018] = OPCODE_MVI_A; // Sign Flag should be 1
	sap_state->ram[0x0019] = 0x03;
	sap_state->ram[0x001A] = OPCODE_MVI_B;
	sap_state->ram[0x001B] = 0x05;
	sap_state->ram[0x001C] = OPCODE_SUB_B;
	sap_state->ram[0x001D] = OPCODE_OUT;
	*/

	sap_state->ram[0x0020] = OPCODE_HLT;
}

void loadprog_700_plus_900(sap_state_t *sap_state) {

	// 700:
	sap_state->ram[0x0000] = OPCODE_MVI_B;
	sap_state->ram[0x0001] = 0x02;
	sap_state->ram[0x0002] = OPCODE_MVI_C;
	sap_state->ram[0x0003] = 0xBC;
	sap_state->ram[0x0004] = OPCODE_OUT;

	// 900:
	sap_state->ram[0x0005] = OPCODE_MVI_D;
	sap_state->ram[0x0006] = 0x03;
	sap_state->ram[0x0007] = OPCODE_MVI_E;
	sap_state->ram[0x0008] = 0x84;
	sap_state->ram[0x0009] = OPCODE_OUT;

	// Add our Lower Bytes:
	sap_state->ram[0x000A] = OPCODE_ADD_C;
	sap_state->ram[0x000B] = OPCODE_ADD_E;
	sap_state->ram[0x000C] = OPCODE_OUT;

	// Save our summed lower byte into L:
	sap_state->ram[0x000D] = OPCODE_MOV_L_A;
	sap_state->ram[0x000E] = OPCODE_MVI_A;
	sap_state->ram[0x000F] = 0x00;
	sap_state->ram[0x0010] = OPCODE_OUT;

	// Upper byte add with carry:
	sap_state->ram[0x0011] = OPCODE_ADC_B;
	sap_state->ram[0x0012] = OPCODE_OUT;
	// Upper byte add:
	sap_state->ram[0x0013] = OPCODE_ADD_D;
	sap_state->ram[0x0014] = OPCODE_OUT;

	// Move High Byte into H:
	sap_state->ram[0x0015] = OPCODE_MOV_H_A;

	sap_state->ram[0x0019] = OPCODE_HLT;
}

void loadprog_inr_test(sap_state_t *sap_state) {

	sap_state->ram[0x0000] = OPCODE_MVI_B;
	sap_state->ram[0x0001] = 0x01;

	sap_state->ram[0x0002] = OPCODE_DCR_B;
	sap_state->ram[0x0003] = OPCODE_OUT;

	sap_state->ram[0x0004] = OPCODE_DCR_B;
	sap_state->ram[0x0005] = OPCODE_OUT;

	sap_state->ram[0x0006] = OPCODE_HLT;
}

int main(int argc, char **argv) {

	FILE *fp;
	unsigned int address;
	unsigned int opcode;

	// Init our SAP Microcontroller:
	sap_state_t *sap_state = init_sap_state();
	if (sap_state == NULL) {
		return 1;
	}

	if (argc >= 2) {
		// Open our file:
		fp = fopen(argv[1], "r");
		if(fp == NULL) {
			printf("Unable to open file\n");
			return 0;
		}

		while (!feof(fp)) {
			while (fscanf(fp, "%x\t%x\n", &address, &opcode) != EOF) {
				sap_state->ram[(uint16_t)address] = (uint8_t)opcode;
				printf("Address: %x Opcode %x\n", address, opcode);
			}
		}
		fclose(fp);
	} else {
		//loadprog_lda_sta_5(sap_state);
		//loadprog_call_ret(sap_state);
		//loadprog_jnz_test(sap_state);
		//loadprog_sign_test(sap_state);
		//loadprog_zero_test(sap_state);
		//loadprog_cma_test(sap_state);
		//loadprog_rar_test(sap_state);
		//loadprog_carry_test(sap_state);
		//loadprog_700_plus_900(sap_state);
		loadprog_inr_test(sap_state);
	}

	printf("\nDumping memory before execution:\n");
	dump_sap_memory(sap_state);

	// Execute SAP:
	execute_sap(sap_state);

	// Dump State:
	printf("\nDumping SAP State ::\n");
	dump_sap_state(sap_state);

	return 0;
}
