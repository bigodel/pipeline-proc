#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(instruction_memory) {
    sc_in<WORD> address;      // input address
    sc_out<WORD> instruction; // output instruction

    SC_CTOR(instruction_memory) {
        SC_METHOD(read_instruction);
        sensitive << address;
        instruction.initialize(0);
    }

    WORD mem[INST_COUNT] = {0}; // initialize memory with zeroes

    // need to divide it by 4 since we are using a 32 bit word (4 bytes)
    void read_instruction() { instruction.write(mem[address.read() / 4]); }
};
