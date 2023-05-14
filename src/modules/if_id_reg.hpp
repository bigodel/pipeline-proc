#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(if_id_reg) {
    sc_in_clk clock;

    // inputs
    sc_in<WORD> inst_in, if_adder_s_in;

    // output
    sc_out<WORD> inst_out, if_adder_s_out;

    // # Control #
    sc_in<bool> if_id_write;

    WORD memory[2] = {0};

    void read() {
        // read
        memory[0] = inst_in.read();
        memory[1] = if_adder_s_in.read();
    }

    void write() {
        // write
            inst_out.write(memory[0]);
            if_adder_s_out.write(memory[1]);
    }

    SC_CTOR(if_id_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};
