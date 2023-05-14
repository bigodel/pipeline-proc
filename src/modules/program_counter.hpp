#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(program_counter) {
    sc_in_clk clock;

    sc_in<WORD> jump_address;
    sc_out<WORD> address;

    // # Control #
    sc_in<bool> pc_write;

    void increment() {
        if (pc_write.read()) {
            int jump_addr = jump_address.read().to_int();

            address.write(jump_addr);
        }
    }

    SC_CTOR(program_counter) {
        SC_METHOD(increment);
        sensitive << clock.pos();
    }
};
