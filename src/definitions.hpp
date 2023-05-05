#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define WORD sc_uint<32>
#define ADDR sc_uint<5>
#define INST_COUNT 256
#define REG_COUNT 32
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
