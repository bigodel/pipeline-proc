#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(if_id_reg) {
    sc_in_clk clock;

    // inputs
    sc_in<WORD> inst_in;
    // sc_in<WORD> program_counter;

    // control signals
    // sc_in<bool> flush;

    // output
    sc_out<WORD> inst_out;

    WORD memory = 0;

    void read() {
        // read
        memory = inst_in.read();
    }

    void write() {
        // write
        inst_out.write(memory);
    }

    SC_CTOR(if_id_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};