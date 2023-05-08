#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(ex_mem_reg) {
    sc_in_clk clock;

    // inputs
    sc_in<WORD> adder_in, alu_result_in, st_mux_in, data2_in;

    // control signals
    // M
    sc_in<bool> alu_zero_in, branch_in, mem_write_in, mem_read_in;
    sc_out<bool> alu_zero_out, branch_out, mem_write_out, mem_read_out;
    // WB
    sc_in<bool> mem_to_reg_in, reg_write_in;
    sc_out<bool> mem_to_reg_out, reg_write_out;

    // output
    sc_out<WORD> adder_out, alu_result_out, st_mux_out, data2_out;

    WORD memory[10] = {0};

    void read() {
        // read
        memory[0] = adder_in.read();
        memory[1] = alu_result_in.read();
        memory[2] = data2_in.read();
        memory[3] = st_mux_in.read();

        // control signals
        // M
        memory[4] = alu_zero_in.read();
        memory[5] = branch_in.read();
        memory[6] = mem_write_in.read();
        memory[7] = mem_read_in.read();
        // WB
        memory[8] = reg_write_in.read();
        memory[9] = mem_to_reg_in.read();
    }

    void write() {
        // write
        adder_out.write(memory[0]);
        alu_result_out.write(memory[1]);
        data2_out.write(memory[2]);
        st_mux_out.write(memory[3]);

        // control signals
        // M
        alu_zero_out.write(memory[4].to_int());
        branch_out.write(memory[5].to_int());
        mem_write_out.write(memory[6].to_int());
        mem_read_out.write(memory[7].to_int());
        // WB
        reg_write_out.write(memory[3].to_int());
        mem_to_reg_out.write(memory[4].to_int());
    }

    SC_CTOR(ex_mem_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};
