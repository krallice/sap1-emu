# sap1-emu

### What is it?
sap1-emu is a rough C emulation of a simple 8Bit computer design documented in *Digital Computer Electronics* by Albert Malvino & Jerald Brown. Implemented based on the book's description of the logical behaviour of the hardware design for the SAP-1 (Simple As Possible). Inspired by Ben Eater's fantastic youtube series on his DIY 8Bit Breadboard Computer.

### Why?
It was a pretty good exercise to gain an understanding how a *basic*, primitive computer functions. It was also an okay exercise with building something slightly non-trivial in C. Played a fair bit about more generic Makefile features aswell. 

### Features:
+ 8bit architecture (8bit Registers, 8Bit Memory Addressing)
+ 16Bytes of RAM (Woo!)
+ 1 Register capable of adding and subtracting from memory. 2nd Register defined but yet unused.
+ 5 Fixed Length Opcodes (4bit Opcode, 4bit Operand). 
+ [Adding and Subtracting](https://www.youtube.com/watch?v=eSBybJGZoCU)

### What's next?
+ *Programming* the computer involves manually flipping bits in RAM *(I guess its more ROM at the moment)* before execution. Might be nice to load code from file or have some defined functions to load instructions
+ Simulate some of the lower level componentry like the Adder or Clock in logic gates
+ Extend to the SAP2/3 etc.. designs 
+ Port Doom?

```
$ bin/sap1emu

Dumping memory before execution:
Address: 0x0    Data: 0x0a
Address: 0x1    Data: 0x1b
Address: 0x2    Data: 0xe0
Address: 0x3    Data: 0xf0
Address: 0x4    Data: 0xee
Address: 0x5    Data: 0xee
Address: 0x6    Data: 0xee
Address: 0x7    Data: 0xee
Address: 0x8    Data: 0xee
Address: 0x9    Data: 0xee
Address: 0xa    Data: 0x50
Address: 0xb    Data: 0x60
Address: 0xc    Data: 0xee
Address: 0xd    Data: 0xee
Address: 0xe    Data: 0xee
Address: 0xf    Data: 0xee

Starting Main Execution Loop
Instruction: 0x0
LDA: Loading Address 0xa (Value: 0x50 / 80) into Accumulator
Instruction: 0x1
ADD: Add Address 0xb (Value: 0x60 / 96) to Accumulator (Old: 0x50 New: 0xb0)
Instruction: 0xe
OUT: Printing Value: 0xb0
Instruction: 0xf
HLT: End of Program.

Dumping SAP State ::
Register A (Accumulator): 0xb0
Register B:             0x00
Program Counter:        0x03
```
