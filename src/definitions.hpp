#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// types
#define WORD sc_bv<32>
#define OP sc_bv<6>
#define ADDR sc_bv<5>
#define IMM sc_bv<16>
#define ALU_OP sc_bv<3>

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
#define OP_AND "000000"
#define OP_OR  "000001"
#define OP_XOR "000010"
#define OP_NOT "000011"
#define OP_CMP "000100"
#define OP_ADD "000101"
#define OP_SUB "000110"
// i-type
#define OP_LD  "000111"
#define OP_ST  "001000"
// j-type
#define OP_J   "001001"
#define OP_JN  "001010"
#define OP_JZ  "001011"

#endif /* DEFINITIONS_H */
