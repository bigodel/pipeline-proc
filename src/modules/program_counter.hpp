#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(program_counter) {
    sc_in_clk clock;
    sc_in<WORD> jump_address;
    sc_out<WORD> address;

    int current_address = -1;

    void increment() {
        int jump_addr = jump_address.read().to_int();

        address.write(WORD(current_address));

        if (jump_addr) {
            current_address = jump_addr;
        } else {
            current_address++;
        }
    }

    SC_CTOR(program_counter) {
        SC_METHOD(increment);
        sensitive << clock.pos();
    }
};
