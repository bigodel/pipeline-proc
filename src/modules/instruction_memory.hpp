#include "definitions.hpp"
#include "systemc.h"
#include <ostream>

SC_MODULE(instruction_memory) {
    sc_in<int> address; // input address
    sc_out<int> opCode, regIndexStart, regIndexTerm, regIndexDest;

    inst mem[INST_COUNT] = {{0, 0, 0, 0}}; // initialize memory

    // need to divide it by 4 since we are using a 32 bit word (4 bytes)
    //void read_instruction() { instruction.write(mem[address.read() / 4]); }

    void read_instruction() { 
        opCode.write(mem[address.read()].opCode); 
        regIndexStart.write(mem[address.read()].regStart); 
        regIndexTerm.write(mem[address.read()].regTerm); 
        regIndexDest.write(mem[address.read()].regDest); 
    }

    SC_CTOR(instruction_memory) {
        SC_METHOD(read_instruction);
        sensitive << address;
        //sensitive << clock.neg(); TODO
        //instruction.initialize(0);
    }
};
