#include <systemc.h>
#include "definitions.hpp"

SC_MODULE(mem_wb_reg) {
    sc_in_clk clock;

    // inputs
    sc_in<WORD> mem_data_in, alu_result_in, st_mux_in;
    sc_out<WORD> mem_data_out, alu_result_out, st_mux_out;

    // control signals
    sc_in<bool> mem_to_reg_in, reg_write_in;
    sc_out<bool> mem_to_reg_out, reg_write_out;

    WORD memory[5] = {0};

    void read() {
        // read
        memory[0] = mem_data_in.read();
        memory[1] = alu_result_in.read();
        memory[2] = st_mux_in.read();

        // control signals
        memory[3] = reg_write_in.read();
        memory[4] = mem_to_reg_in.read();
    }

    void write() {
        // write
        mem_data_out.write(memory[0]);
        alu_result_out.write(memory[1]);
        st_mux_out.write(memory[2]);

        // control signals
        reg_write_out.write(memory[3].to_int());
        mem_to_reg_out.write(memory[4].to_int());
    }

    SC_CTOR(mem_wb_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};
