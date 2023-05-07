#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// types
#define WORD sc_uint<32>
#define OP sc_uint<6>
#define ADDR sc_uint<5>
#define IMM sc_uint<16>

// constants
#define INST_COUNT 256
#define REG_COUNT 32

// alu ops
#define ADD 0b000
#define SUB 0b001
#define AND 0b010
#define OR  0b011
#define XOR 0b100
#define NOT 0b101
#define CMP 0b110

// opcodes
// r-type
#define OP_AND 0b0000
#define OP_OR  0b0001
#define OP_XOR 0b0010
#define OP_NOT 0b0011
#define OP_CMP 0b0100
#define OP_ADD 0b0101
#define OP_SUB 0b0110
// i-type
#define OP_LD  0b0111
#define OP_ST  0b1000
// j-type
#define OP_J   0b1001
#define OP_JN  0b1010
#define OP_JZ  0b1011

#endif /* DEFINITIONS_H */
