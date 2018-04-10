# sap2-emu

### What is it?
sap2-emu is a rough C emulation of the SAP-2, the first extension to the SAP-1. It features a larger bus width, an upgrade to 8bit opcodes, direct memory addressing for storing data to RAM and a more flexible Program Counter.

### Features:
+ Registers still 8Bit; but increased to 3 (A,B,C).
+ 16Bit Memory Addressing (0x0000 - 0xFFFF aka 64K RAM). 
+ Opcode to store the Accumulator straight to a direct memory address (STA).
+ Multi-Byte Instructions (up to 3 bytes).
+ 2 Flip/Flop Flags (Sign and Zero).
+ JUMP and CALL instructions allowing more complex program flow *(loops and subroutines)*

### What's it missing?:
+ A stack. Memory protection (Everything is direct access). Paging.
+ Larger Registers?
+ Register Overflow Detection *Carry Flags?*

### What's next?
+ Replace Switch statement with a jump table (aka jumping directly to function pointers?)
+ Better way to load code for it?
+ SAP-3

### Opcodes:

Mnemonic        | Opcode (8bits)   | Operand Size (Bytes) | Description
--- | --- | --- | ----
NOP | 0x00 | --- | NULL Instruction
HLT | 0x76 | --- | Halt Instruction Flow (End Program)
--- | --- | --- | ----
MVI_A | 0x3E | 1 | Load Immediate Byte into Register A
MVI_B | 0x06 | 1 | Load Immediate Byte into Register B
MVI_C | 0x0E | 1 | Load Immediate Byte into Register C
--- | --- | --- | ----
MOV_A_B | 0x78 | --- | Load Register B into Register A. Register B value remains.
MOV_A_C | 0x79 | --- | Load Register C into Register A. Register C value remains.
MOV_B_A | 0x47 | --- | Load Register A into Register B. Register A value remains.
MOV_B_C | 0x41 | --- | Load Register C into Register B. Register C value remains.
MOV_C_A | 0x4F | --- | Load Register A into Register C. Register A value remains.
MOV_C_B | 0x48 | --- | Load Register B into Register C. Register B value remains.
--- | --- | --- | ----
LDA | 0x3A | 2 | 2 Byte Operand denotes a Memory Address. Load the 8bit Value stored at @Address into A. 
STA | 0x32 | 2 | Store A's value into the Memory Address denoted by the 2 Byte Operand. 
--- | --- | --- | ----
JMP | 0xC3 | 2 | Unconditionally jump *(Set the program counter)* to the 2 Byte Memory Address denoted by the Operand.
JZ | 0xCA | 2 | Jump to @Address if Zero Flag is 1
JNZ | 0xC2 | 2 | Jump to @Address if Zero Flag is 0
JM | 0xFA | 2 | Jump to @Address Sign Flag is 1
--- | --- | --- | ----
CALL | 0xCD | 2 | Save PC of next instruction into 0xFFFE and 0xFFFF. Jump to Memory Address denoted by the Operand.
RET  | 0xC9 | --- | Load 0xFFFE / 0xFFFF into the PC *(Return to Instruction after Call)*
--- | --- | --- | ----
ADD_B | 0x80 | -- | Add the Value in Register B to A
ADD_C | 0x81 | -- | Add the Value in Register C to A
SUB_B | 0x90 | -- | Subtract the Value in Register B from A
SUB_C | 0x91 | -- | Subtract the Value in Register C from A
INR_A | 0x3C | -- | Increment A
INR_B | 0x04 | -- | Increment B
INR_C | 0x0C | -- | Increment C
DCR_A | 0x3D | -- | Decrement A
DCR_B | 0x05 | -- | Decrement B
DCR_C | 0x0D | -- | Decrement C
--- | --- | --- | ----
CMA | 0x2F | --- | Complement the Accumulator *(Flip every bit in A)*
ANA_B | 0xA0 | --- | A = A AND B
ANA_C | 0xA1 | --- | A = A AND C
ANI | 0xE6 | 1 | A = A AND Immediate Byte
ORA_B | 0xB0 | -- | A = A OR B
ORA_C | 0xB1 | -- | A = A OR C
ORI | 0xF6 | 1 | A = A OR Immediate Byte
XRA_B | 0xA8 | --- | A = A XOR B
XRA_C | 0xA9 | --- | A = A XOR C
XRI | 0xEE | 1 | A = A XOR Immediate Byte
--- | --- | --- | ----
RAR | 0x1F | --- | Bitwise Rotate Accumulator to the Right 0000 0101 -> 1000 0010
RAL | 0x17 | --- | Bitwise Rotate Accumulator to the Left 1100 0001 -> 1000 0011

### Flags:

The SAP-2 has 2 Flags. These are used for condition checking for the various JMP Commands:

Flag | Condition
--- | ---
Zero Flag | Flag == 1 IF A == 0
Sign Flag | Flag == 1 IF A < 0

The Flags are set/unset depending on the value of A at the completion of the following instructions:

Instruction | Flags Affected
--- | ---
ADD | S, Z
SUB | S, Z
INR | S, Z
DCR | S, Z
ANA | S, Z
ORA | S, Z
XRA | S, Z
ANI | S, Z
ORI | S, Z
XRI | S, Z

### Programming it:
```

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
        //      A = A + B
        //      C--
        sap_state->ram[0x0006] = OPCODE_ADD_B;
        sap_state->ram[0x0007] = OPCODE_DCR_C;
        sap_state->ram[0x0008] = OPCODE_JNZ;
        sap_state->ram[0x0009] = 0x06;
        sap_state->ram[0x000A] = 0x00;

        sap_state->ram[0x000B] = OPCODE_HLT;
 ```

### Running it:
```
$ bin/sap2-emu 

Dumping memory before execution:
Address: 0x0000 Data: 0x0e
Address: 0x0001 Data: 0x05
Address: 0x0002 Data: 0x06
Address: 0x0003 Data: 0x07
Address: 0x0004 Data: 0x3e
Address: 0x0005 Data: 0x00
Address: 0x0006 Data: 0x80
Address: 0x0007 Data: 0x0d
Address: 0x0008 Data: 0xc2
Address: 0x0009 Data: 0x06
Address: 0x000a Data: 0x00
Address: 0x000b Data: 0x76

Starting Main Execution Loop
Instruction: 0x0e MVI: Move Value 0x05 into Register C
Instruction: 0x06 MVI: Move Value 0x07 into Register B
Instruction: 0x3e MVI: Move Value 0x00 into Register A
Instruction: 0x80 ADD: Adding Value (0x07 / 7) from Register B to Accumulator (0x00 / 0) with Result (0x07 / 7)
Instruction: 0x0d DCR: Decrementing Value (0x05 / 5) in Register C to Value (0x04 / 4)
Instruction: 0xc2 JNZ: Setting PC to Address 0006
Instruction: 0x80 ADD: Adding Value (0x07 / 7) from Register B to Accumulator (0x07 / 7) with Result (0x0e / 14)
Instruction: 0x0d DCR: Decrementing Value (0x04 / 4) in Register C to Value (0x03 / 3)
Instruction: 0xc2 JNZ: Setting PC to Address 0006
Instruction: 0x80 ADD: Adding Value (0x07 / 7) from Register B to Accumulator (0x0e / 14) with Result (0x15 / 21)
Instruction: 0x0d DCR: Decrementing Value (0x03 / 3) in Register C to Value (0x02 / 2)
Instruction: 0xc2 JNZ: Setting PC to Address 0006
Instruction: 0x80 ADD: Adding Value (0x07 / 7) from Register B to Accumulator (0x15 / 21) with Result (0x1c / 28)
Instruction: 0x0d DCR: Decrementing Value (0x02 / 2) in Register C to Value (0x01 / 1)
Instruction: 0xc2 JNZ: Setting PC to Address 0006
Instruction: 0x80 ADD: Adding Value (0x07 / 7) from Register B to Accumulator (0x1c / 28) with Result (0x23 / 35)
Instruction: 0x0d DCR: Decrementing Value (0x01 / 1) in Register C to Value (0x00 / 0)
Instruction: 0xc2 JNZ: Zero Flag Not Set. No Jump taken
Instruction: 0x76 HLT: End of Program.

Dumping SAP State ::
Register A (Accumulator): 0x23
Register B:             0x07
Register C:             0x00
Zero Flag:              1
Sign Flag:              0
Program Counter:        0x000b
```
