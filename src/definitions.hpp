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

#endif /* DEFINITIONS_H */
