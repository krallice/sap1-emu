#ifndef OPCODES_H
#define OPCODES_H

#define OPCODE_DEBUG 1

#define OPCODE_NOP 0x00 // No Execution
#define OPCODE_HLT 0x76 // Halt Instruction Flow

#define OPCODE_OUT 0xD3 // OUT (Incomplete Implementation, currently used as a diag dump)

// MVI_<Register> <Value>
// Move Immediate Next Byte Value into <Register>. 2 Byte Instruction
#define OPCODE_MVI_A 0x3E
#define OPCODE_MVI_B 0x06
#define OPCODE_MVI_C 0x0E
#define OPCODE_MVI_D 0x16
#define OPCODE_MVI_E 0x1E
#define OPCODE_MVI_H 0x26
#define OPCODE_MVI_L 0x2E

// MOV <1st Register> <--- <2nd Register>
// Move Byte Value in 2nd Register into First. 1 Byte Instruction
#define OPCODE_MOV_A_B 0x78 
#define OPCODE_MOV_A_C 0x79
#define OPCODE_MOV_A_D 0x7A 
#define OPCODE_MOV_A_E 0x7B
#define OPCODE_MOV_A_H 0x7C 
#define OPCODE_MOV_A_L 0x7D

#define OPCODE_MOV_B_A 0x47
#define OPCODE_MOV_B_C 0x41
#define OPCODE_MOV_B_D 0x42
#define OPCODE_MOV_B_E 0x43
#define OPCODE_MOV_B_H 0x44
#define OPCODE_MOV_B_L 0x45

#define OPCODE_MOV_C_A 0x4F
#define OPCODE_MOV_C_B 0x48
#define OPCODE_MOV_C_D 0x4A
#define OPCODE_MOV_C_E 0x4B
#define OPCODE_MOV_C_H 0x4C
#define OPCODE_MOV_C_L 0x4D

#define OPCODE_MOV_D_A 0x57
#define OPCODE_MOV_D_B 0x50
#define OPCODE_MOV_D_C 0x51
#define OPCODE_MOV_D_E 0x53
#define OPCODE_MOV_D_H 0x54
#define OPCODE_MOV_D_L 0x55

#define OPCODE_MOV_E_A 0x5F
#define OPCODE_MOV_E_B 0x58
#define OPCODE_MOV_E_C 0x59
#define OPCODE_MOV_E_D 0x5A
#define OPCODE_MOV_E_H 0x5C
#define OPCODE_MOV_E_L 0x5D

#define OPCODE_MOV_H_A 0x67
#define OPCODE_MOV_H_B 0x60
#define OPCODE_MOV_H_C 0x61
#define OPCODE_MOV_H_D 0x62
#define OPCODE_MOV_H_E 0x63
#define OPCODE_MOV_H_L 0x65

#define OPCODE_MOV_L_A 0x6F
#define OPCODE_MOV_L_B 0x68
#define OPCODE_MOV_L_C 0x69
#define OPCODE_MOV_L_D 0x6A
#define OPCODE_MOV_L_E 0x6B
#define OPCODE_MOV_L_H 0x6C

#define OPCODE_LDA 0x3A // Load the following 16Bit (2 Byte) Memory Address into the Accumulator. 3 Byte Instruction.
#define OPCODE_STA 0x32 // Store the Accumulator into the following 16Bit (2 Byte) Memory Address. 3 Byte Instruction.

#define OPCODE_JMP 0xC3 // Unconditional Jump
#define OPCODE_JZ  0xCA // Jmp if Flag_Zero -= 0
#define OPCODE_JNZ  0xC2 // Jmp if Flag_Zero != 0
#define OPCODE_JM 0xFA // Jmp if Flag_Sign == 1

#define OPCODE_CALL 0xCD // Call Function
#define OPCODE_RET  0xC9 // Return to Caller

#define OPCODE_ADD_B 0x80 // Add the value in Register (B|C) to Accumulator
#define OPCODE_ADD_C 0x81

#define OPCODE_SUB_B 0x90 // Substract the value in Reg (B|C) from Accumulator
#define OPCODE_SUB_C 0x91

#define OPCODE_INR_A 0x3C // Increment Registers by 1:
#define OPCODE_INR_B 0x04
#define OPCODE_INR_C 0x0C

#define OPCODE_DCR_A 0x3D // Decrement Registers by 1:
#define OPCODE_DCR_B 0x05
#define OPCODE_DCR_C 0x0D

#define OPCODE_CMA 0x2F // Complement the Accumulator
#define OPCODE_ANA_B 0xA0 // A & B
#define OPCODE_ANA_C 0xA1 // A & C
#define OPCODE_ANI 0xE6 // A & Immediate Byte

#define OPCODE_ORA_B 0xB0 // A | B
#define OPCODE_ORA_C 0xB1 // A | C
#define OPCODE_ORI 0xF6 // A | Immediate Byte

#define OPCODE_XRA_B 0xA8 // A ^ B
#define OPCODE_XRA_C 0xA9 // A ^ C
#define OPCODE_XRI 0xEE // A ^ Immediate Byte

#define OPCODE_RAR 0x1F // Rotate Right
#define OPCODE_RAL 0x17 // Rotate Left

void do_opcode_hlt(sap_state_t *sap_state);
void do_opcode_nop(sap_state_t *sap_state);
void do_opcode_out(sap_state_t *sap_state);
void do_opcode_mvi(sap_state_t *sap_state, int8_t *dst_reg, char *dst_reg_name);
void do_opcode_sta(sap_state_t *sap_state);
void do_opcode_lda(sap_state_t *sap_state);
void do_opcode_add(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name);
void do_opcode_sub(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name);
void do_opcode_inr(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name);
void do_opcode_dcr(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name);
void do_opcode_mov(sap_state_t *sap_state, int8_t *dst_reg, char *dst_reg_name, int8_t *src_reg, char *src_reg_name);
void do_opcode_jmp(sap_state_t *sap_state);
void do_opcode_jz(sap_state_t *sap_state);
void do_opcode_jnz(sap_state_t *sap_state);
void do_opcode_jm(sap_state_t *sap_state);
void do_opcode_call(sap_state_t *sap_state);
void do_opcode_ret(sap_state_t *sap_state);
void do_opcode_cma(sap_state_t *sap_state);
void do_opcode_ana(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name);
void do_opcode_ani(sap_state_t *sap_state);
void do_opcode_ora(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name);
void do_opcode_ori(sap_state_t *sap_state);
void do_opcode_xra(sap_state_t *sap_state, int8_t *src_reg, char *src_reg_name);
void do_opcode_xri(sap_state_t *sap_state);
void do_opcode_ral(sap_state_t *sap_state);
void do_opcode_rar(sap_state_t *sap_state);

#endif
