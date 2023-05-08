#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(ex_mem_reg) {
    sc_in_clk clock;

    // inputs 
    sc_in<WORD> adder_in, alu_result_in,
    st_mux_in, data2_in;
    sc_in<bool> alu_zero_in;
    // sc_in<WORD> program_counter;

    // control signals
    // sc_in<bool> flush;

    // output
    sc_out<WORD> adder_out, alu_result_out, st_mux_out, data2_out;
    sc_out<bool> alu_zero_out;

    WORD memory[4] = {0};
    bool alu_zero_mem = 0;

    void read() {
        // read
        memory[0] = adder_in.read();
        memory[1] = alu_result_in.read();
        memory[2] = data2_in.read();
        memory[3] = st_mux_in.read();

        alu_zero_mem = alu_zero_in.read();
    }

    void write() {
        // write
        adder_out.write(memory[0]);
        alu_result_out.write(memory[1]);
        data2_out.write(memory[2]);
        st_mux_out.write(memory[3]);

        alu_zero_out.write(alu_zero_mem);
    }

    SC_CTOR(ex_mem_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};
