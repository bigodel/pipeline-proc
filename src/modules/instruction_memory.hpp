#include "definitions.hpp"
#include "systemc.h"
#include <string>
#include <ostream>

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
