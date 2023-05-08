#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(id_ex_reg) {
    sc_in_clk clock;

    // inputs
    sc_in<WORD> data1_in, data2_in;
    sc_in<WORD> inst_15_0_in, inst_20_16_in, inst_15_11_in;
    sc_in<WORD> adder_in;

    // control signals
    // EX
    sc_in<bool> reg_dst_in, alu_src_in;
    sc_in<ALU_OP> alu_op_in;
    sc_out<bool> reg_dst_out, alu_src_out;
    sc_out<ALU_OP> alu_op_out;
    // M
    sc_in<bool> alu_zero_in, branch_in, mem_write_in, mem_read_in;
    sc_out<bool> alu_zero_out, branch_out, mem_write_out, mem_read_out;
    // WB
    sc_in<bool> mem_to_reg_in, pc_src_in;
    sc_out<bool> mem_to_reg_out, pc_src_out;

    // output
    sc_out<WORD> data1_out, data2_out;
    sc_out<WORD> inst_15_0_out, inst_20_16_out, inst_15_11_out;
    sc_out<WORD> adder_out;

    WORD memory[15] = {0};

    void read() {
        // read
        memory[0] = data1_in.read();
        memory[1] = data2_in.read();
        memory[2] = inst_15_0_in.read();
        memory[3] = inst_20_16_in.read();
        memory[4] = inst_15_11_in.read();
        memory[5] = adder_in.read();

        // control signals
        // EX
        memory[6] = reg_dst_in.read();
        memory[7] = alu_src_in.read();
        memory[8] = alu_op_in.read();
        // M
        memory[9] = alu_zero_in.read();
        memory[10] = branch_in.read();
        memory[11] = mem_write_in.read();
        memory[12] = mem_read_in.read();
        // WB
        memory[13] = pc_src_in.read();
        memory[14] = mem_to_reg_in.read();
    }

    void write() {
        // write
        data1_out.write(memory[0]);
        data2_out.write(memory[1]);
        inst_15_0_out.write(memory[2]);
        inst_20_16_out.write(memory[3]);
        inst_15_11_out.write(memory[4]);
        adder_out.write(memory[5]);

        // control signals
        // EX
        reg_dst_out.write(memory[6].to_int());
        alu_src_out.write(memory[7].to_int());
        alu_op_out.write(memory[8]);
        // M
        alu_zero_out.write(memory[9].to_int());
        branch_out.write(memory[10].to_int());
        mem_write_out.write(memory[11].to_int());
        mem_read_out.write(memory[12].to_int());
        // WB
        pc_src_out.write(memory[13].to_int());
        mem_to_reg_out.write(memory[14].to_int());
    }

    SC_CTOR(id_ex_reg) {
        SC_METHOD(read);
        sensitive << clock.pos();

        SC_METHOD(write);
        sensitive << clock.neg();
    }
};
