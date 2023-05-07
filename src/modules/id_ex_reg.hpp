#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(id_ex_reg) {
    sc_in_clk clock;

    // inputs
    sc_in<WORD> data1_in, data2_in;
    sc_in<WORD> inst_15_0_in, inst_20_16_in, inst_15_11_in;
    sc_in<WORD> adder_in;
    // sc_in<WORD> program_counter;

    // control signals
    // sc_in<bool> flush;

    // output
    sc_out<WORD> data1_out, data2_out;
    sc_out<WORD> inst_15_0_out, inst_20_16_out, inst_15_11_out;
    sc_out<WORD> adder_out;

    WORD memory[6] = {0};

    void read() {
        // read
        memory[0] = data1_in.read();
        memory[1] = data2_in.read();
        memory[2] = inst_15_0_in.read();
        memory[3] = inst_20_16_in.read();
        memory[4] = inst_15_11_in.read();
        memory[5] = adder_in.read();
    }

    void write() {
        // write
        data1_out.write(memory[0]);
        data2_out.write(memory[1]);
        inst_15_0_out.write(memory[2]);
        inst_20_16_out.write(memory[3]);
        inst_15_11_out.write(memory[4]);
        adder_out.write(memory[5]);
    }

    SC_CTOR(id_ex_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};
