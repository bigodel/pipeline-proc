#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(program_counter) {
    sc_in_clk clock;

    sc_in<WORD> jump_address;
    sc_out<WORD> address;

    // # Control #
    sc_in<bool> pc_write;

    int jump_addr;

    void read_addr() {
        if (pc_write.read()) {
            jump_addr = jump_address.read().to_int();
        }
    }

    void write_addr() {
        if (pc_write.read()) {
            address.write(jump_addr);
        }
    }

    SC_CTOR(program_counter) {
        SC_METHOD(read_addr);
        sensitive << clock.pos();

        SC_METHOD(write_addr);
        sensitive << clock.neg();
    }
};
