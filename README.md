# sap1-emu

### See Also:
##### [SAP-2](./sap2/) - Second Generation with 64K RAM, Loops and Functions

### What is it?
sap1-emu is a rough C emulation of a simple 8Bit computer design documented in *Digital Computer Electronics* by Albert Malvino & Jerald Brown. Implemented based on the book's description of the logical behaviour of the hardware design for the SAP-1 (Simple As Possible). Inspired by Ben Eater's fantastic youtube series on his DIY 8Bit Breadboard Computer.

### Why?
It was a pretty good exercise to gain an understanding how a *basic*, primitive computer functions. It was also an okay exercise in building something slightly non-trivial with C. Played a fair bit with generic Makefile features aswell to get it nice and scalable.

### Features:
+ 8bit architecture (8bit Registers, 8Bit Memory Sizes)
+ 16Bytes of RAM. 4 Bits of Addressing (Woo!)
+ 1 Register capable of adding and subtracting from memory. 2nd Register defined but yet unused.
+ 5 Fixed Length Opcodes (4bit Opcode, 4bit Operand). 
+ [Adding and Subtracting](https://www.youtube.com/watch?v=eSBybJGZoCU)

### What's next?
+ *Programming* the computer involves manually flipping bits in RAM *(I guess its more ROM at the moment)* before execution. Might be nice to load code from file or have some defined functions to load instructions
+ Simulate some of the lower level componentry like the Adder or Clock in logic gates
+ Extend to the SAP2/3 etc.. designs 
+ Port Doom?

### Opcodes:

Mnemonic        | Opcode (4bits)   | Operand (4bits)  | Description
--- | --- | --- | ----
LDA             | 0x0              | 4bit Memory Address | Load the value of memory @Address into Accumulator A
ADD             | 0x1              | 4bit Memory Address | Add the value of memory @Address to Accumulator A
SUB             | 0x2              | 4bit Memory Address | Subtract the value of memory @Address to Accumulator A
OUT             | 0xE              | NULL | Send the value of the Accumulator A to the Output Pin
HLT             | 0xF              | NULL | End execution of program

### Programming it:
```
        // Operations:
        sap_state->ram[0x0] = (OPCODE_LDA << 4) + 0xA;
        sap_state->ram[0x1] = (OPCODE_ADD << 4) + 0xB;
        sap_state->ram[0x2] = OPCODE_OUT << 4;
        sap_state->ram[0x3] = OPCODE_HLT << 4;

        // Data Section:
        sap_state->ram[0xA] = 0x50;
        sap_state->ram[0xB] = 0x60;
```

### Running it:
```
$ bin/sap1emu 

Dumping memory before execution:
Address: 0x0    Data: 0x0a
Address: 0x1    Data: 0x1b
Address: 0x2    Data: 0xe0
Address: 0x3    Data: 0xf0
Address: 0x4    Data: 0x00
Address: 0x5    Data: 0x00
Address: 0x6    Data: 0x00
Address: 0x7    Data: 0x00
Address: 0x8    Data: 0x00
Address: 0x9    Data: 0x00
Address: 0xa    Data: 0x03
Address: 0xb    Data: 0x02
Address: 0xc    Data: 0x00
Address: 0xd    Data: 0x00
Address: 0xe    Data: 0x00
Address: 0xf    Data: 0x00

Starting Main Execution Loop
Instruction: 0x0
LDA: Loading Address 0xa (Value: 0x03 / 3) into Accumulator
Instruction: 0x1
ADD: Add Address 0xb (Value: 0x02 / 2) to Accumulator (Old: 0x03 New: 0x05)
Instruction: 0xe
OUT: Printing Value: 0x05
Instruction: 0xf
HLT: End of Program.

Dumping SAP State ::
Register A (Accumulator): 0x05
Register B:             0x00
Program Counter:        0x03
```

### License:

SAP1/2/3 Licensed under the [GPLv2 License](./LICENSE).
