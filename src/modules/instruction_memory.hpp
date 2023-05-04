#include "definitions.hpp"
#include "systemc.h"
#include <string>
#include <ostream>

using namespace std;

struct inst {
    string opCode;
    int regStart, regTerm, regDest;

    // Constructor
    inst(string opCode = "", int regStart = 0, int regTerm = 0, int regDest = 0) 
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
// Needed for tracing
inline void sc_trace(sc_trace_file*& f, const inst& val, std::string name) { 
    sc_trace(f, val.opCode, name + ".opCode");
    sc_trace(f, val.regStart, name + ".RefStart");
    sc_trace(f, val.regTerm, name + ".RegTerm");
    sc_trace(f, val.regDest, name + ".RegDest");
}

inline void sc_trace(sc_trace_file*& f, const string& val, std::string name) { 
    sc_trace(f, val, name + ".string");
}

SC_MODULE(instruction_memory) {
    sc_in<int> address; // input address
    sc_out<inst> instruction;

    inst mem[INST_COUNT] = {{"START", 0, 0, 0}}; // initialize memory

    // need to divide it by 4 since we are using a 32 bit word (4 bytes)
    //void read_instruction() { instruction.write(mem[address.read() / 4]); }

    void read_instruction() { instruction.write(mem[address.read()]); }

    SC_CTOR(instruction_memory) {
        SC_METHOD(read_instruction);
        sensitive << address;
        //sensitive << clock.neg(); TODO
        //instruction.initialize(0);
    }
};
