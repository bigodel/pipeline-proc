#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define WORD sc_uint<32>
#define ADDR sc_uint<5>
#define INST_COUNT 256
#define REG_COUNT 32
using namespace std;

// INSTRUCTION TYPE
struct inst {
    int opCode;
    int regStart, regTerm, regDest;

    // Constructor
    inst(int opCode = 0, int regStart = 0, int regTerm = 0, int regDest = 0) 
    : opCode(opCode), regStart(regStart), regTerm(regTerm), regDest(regDest) {}

    // Assignment operator, needed for read() write()
    inst& operator=(const inst& rhs) { 
        opCode = rhs.opCode;
        regStart = rhs.regStart;
        regTerm = rhs.regTerm;
        regDest = rhs.regDest;
        return *this;
    }

    // Equality operator, needed for value_changed_event()
    bool operator==(const inst& rhs) { 
        return opCode == rhs.opCode &&
        regStart == rhs.regStart &&
        regTerm == rhs.regTerm &&
        regDest == rhs.regDest;
    }
};

// Streaming output, needed for printing
std::ostream& operator<<(std::ostream& os, const inst& val) {
    os << "opCode = " << val.opCode <<
        "; regStart = " << val.regStart <<
        "; regTerm = " << val.regTerm << 
        "; regDest = " << val.regDest << endl;
    return os;
}

// TODO: fix stirng tracing
// Needed for tracing
inline void sc_trace(sc_trace_file*& f, const inst& val, std::string name) { 
    sc_trace(f, val.opCode, name + ".opCode");
    sc_trace(f, val.regStart, name + ".regStart");
    sc_trace(f, val.regTerm, name + ".regTerm");
    sc_trace(f, val.regDest, name + ".regDest");
}

#endif /* DEFINITIONS_H */
