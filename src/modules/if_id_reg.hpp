#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(if_id_reg) {
    sc_in_clk clock;

    // inputs
    sc_in<WORD> inst_in, if_adder_s_in;

    // output
    sc_out<WORD> inst_out, if_adder_s_out;

    // # Control #
    sc_in<bool> if_id_write;
    sc_in<bool> flush;

    WORD memory[2] = {0};

    void read() {
        if (flush.read()) {
            memory[0] = WORD(0);
            memory[1] = WORD(0);
        } else {
            memory[0] = inst_in.read();
            memory[1] = if_adder_s_in.read();
        }
    }

    void write() {
        if (if_id_write.read()) {
            inst_out.write(memory[0]);
            if_adder_s_out.write(memory[1]);
        } else {
            inst_out.write(WORD(0));
            // if_adder_s_out.write(WORD(0));
        }
    }

    SC_CTOR(if_id_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};
