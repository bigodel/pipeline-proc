#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// types
#define WORD sc_uint<32>
#define OP sc_uint<6>
#define ADDR sc_uint<5>
#define IMM sc_uint<16>
#define ALU_OP sc_int<3>

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
#define OP_AND 0b000000
#define OP_OR  0b000001
#define OP_XOR 0b000010
#define OP_NOT 0b000011
#define OP_CMP 0b000100
#define OP_ADD 0b000101
#define OP_SUB 0b000110
// i-type
#define OP_LD  0b000111
#define OP_ST  0b001000
// j-type
#define OP_J   0b001001
#define OP_JN  0b001010
#define OP_JZ  0b001011

using namespace std;

// INSTRUCTION TYPE
struct inst {
    int opCode, regRead1, regRead2, regWrite;

    // Constructor
    inst(int opCode = 0, int regRead1 = 0, int regRead2 = 0, int regWrite = 0) 
    : opCode(opCode), regRead1(regRead1), regRead2(regRead2), regWrite(regWrite) {}

    // Assignment operator, needed for read() write()
    inst& operator=(const inst& rhs) { 
        opCode = rhs.opCode;
        regRead1 = rhs.regRead1;
        regRead2 = rhs.regRead2;
        regWrite = rhs.regWrite;
        return *this;
    }

    // Equality operator, needed for value_changed_event()
    bool operator==(const inst& rhs) { 
        return opCode == rhs.opCode &&
        regRead1 == rhs.regRead1 &&
        regRead2 == rhs.regRead2 &&
        regWrite == rhs.regWrite;
    }
};

// Streaming output, needed for printing
std::ostream& operator<<(std::ostream& os, const inst& val) {
    os << "opCode = " << val.opCode <<
        "; regRead1 = " << val.regRead1 <<
        "; regRead2 = " << val.regRead2 << 
        "; regWrite = " << val.regWrite << endl;
    return os;
}

// TODO: fix stirng tracing
// Needed for tracing
inline void sc_trace(sc_trace_file*& f, const inst& val, std::string name) { 
    sc_trace(f, val.opCode, name + ".opCode");
    sc_trace(f, val.regRead1, name + ".regRead1");
    sc_trace(f, val.regRead2, name + ".regRead2");
    sc_trace(f, val.regWrite, name + ".regWrite");
}

#endif /* DEFINITIONS_H */
