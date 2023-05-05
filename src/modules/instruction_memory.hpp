#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(instruction_memory) {
    sc_in<int> address;
    sc_out<int> opCode, regRead1, regRead2, regWrite;

    inst memory[INST_COUNT] = {{9, 9, 9, 9}};

    void read() { 
        opCode.write(memory[address.read()].opCode); 
        regRead1.write(memory[address.read()].regRead1); 
        regRead2.write(memory[address.read()].regRead2); 
        regWrite.write(memory[address.read()].regWrite); 
    }

    SC_CTOR(instruction_memory) {
        SC_METHOD(read);
        sensitive << address;
    }
};
