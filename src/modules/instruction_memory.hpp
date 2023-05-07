#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(instruction_memory) {
    sc_in<int> address;
    sc_out<WORD> inst;

    WORD memory[INST_COUNT] = {0};

    void read() {
        inst.write(memory[address.read()]);
    }

    SC_CTOR(instruction_memory) {
        SC_METHOD(read);
        sensitive << address;
    }
};
