#include <bitset>
#include <fstream>
#include <map>
#include <string>
#include <systemc.h>

using namespace std;

// Include modules
#include "definitions.hpp"
#include "modules/adder.hpp"
#include "modules/alu.hpp"
#include "modules/data_memory.hpp"
#include "modules/ex_mem_reg.hpp"
#include "modules/id_ex_reg.hpp"
#include "modules/if_id_reg.hpp"
#include "modules/instruction_memory.hpp"
#include "modules/mem_wb_reg.hpp"
#include "modules/mux.hpp"
#include "modules/program_counter.hpp"
#include "modules/register_file.hpp"
#include "modules/shift_left_2.hpp"
#include "modules/sign_extender.hpp"

SC_MODULE(testbench) {
    sc_in_clk clock;

    void test() {
        for (size_t i = 0; i < 10; i++) {
            wait();
        }

        sc_stop();
    }

    SC_CTOR(testbench) {
        SC_THREAD(test);
        sensitive << clock.pos();
    }
};

int sc_main(int argc, char *argv[]) {
    // ### CLOCK ###
    sc_clock clock("clock", 100, SC_NS, 0.5);

    // ### SIGNALS (WIRES) ###
    // IF stage ----------------------------------------------------------------
    // IF stage mux
    sc_signal<WORD> if_mux_jump_address;
    // program counter
    sc_signal<WORD> pc_address;
    // instruction memory
    sc_signal<WORD> im_inst;
    // IF stage adder
    sc_signal<WORD> if_adder_4{"constant_4", WORD(4)};
    sc_signal<WORD> if_adder_s;
    // IF stage ----------------------------------------------------------------

    // IF/ID register
    sc_signal<WORD> if_id_inst_out;
    sc_signal<WORD> if_id_address_out;

    // ID stage ----------------------------------------------------------------
    // register file
    sc_signal<WORD> rf_data1;
    sc_signal<WORD> rf_data2;
    // sign extender
    sc_signal<WORD> se_output;
    // control signals
    // EX
    sc_signal<ALU_OP> alu_op;
    sc_signal<bool> alu_src;
    sc_signal<bool> reg_dst;
    // M
    sc_signal<bool> branch;
    sc_signal<bool> mem_write;
    sc_signal<bool> mem_read;
    // WB
    sc_signal<bool> mem_to_reg;
    sc_signal<bool> reg_write;
    // ID stage ----------------------------------------------------------------

    // ID/EX register
    sc_signal<WORD> id_ex_adder_out;
    sc_signal<WORD> id_ex_data1_out;
    sc_signal<WORD> id_ex_data2_out;
    sc_signal<WORD> id_ex_inst_15_0_out;
    sc_signal<WORD> id_ex_inst_20_16_out;
    sc_signal<WORD> id_ex_inst_15_11_out;
    // control signals
    // EX
    sc_signal<ALU_OP> id_ex_alu_op_out;
    sc_signal<bool> id_ex_alu_src_out;
    sc_signal<bool> id_ex_reg_dst_out;
    // M
    sc_signal<bool> id_ex_branch_out;
    sc_signal<bool> id_ex_mem_write_out;
    sc_signal<bool> id_ex_mem_read_out;
    // WB
    sc_signal<bool> id_ex_mem_to_reg_out;
    sc_signal<bool> id_ex_reg_write_out;

    // EX stage ----------------------------------------------------------------
    // adder
    sc_signal<WORD> ex_adder_s;
    // alu
    sc_signal<WORD> alu_result;
    sc_signal<bool> alu_zero;
    // alu mux
    sc_signal<WORD> ex_alu_mux_out;
    // sw mux reg (used for choosing the register to write on ST instruction)
    sc_signal<WORD> ex_st_mux_out;
    // shift left 2
    sc_signal<WORD> sl2_out;
    // EX stage ----------------------------------------------------------------

    // EX/MEM register
    sc_signal<WORD> ex_mem_alu_adder_s_out;
    sc_signal<WORD> ex_mem_adder_out;
    sc_signal<WORD> ex_mem_alu_result_out;
    sc_signal<WORD> ex_mem_st_mux_out;
    sc_signal<WORD> ex_mem_data2_out;
    // control signals
    // M
    sc_signal<bool> ex_mem_branch_out;
    sc_signal<bool> ex_mem_alu_zero_out;
    sc_signal<bool> ex_mem_mem_write_out;
    sc_signal<bool> ex_mem_mem_read_out;
    // WB
    sc_signal<bool> ex_mem_mem_to_reg_out;
    sc_signal<bool> ex_mem_reg_write_out;

    // MEM stage ---------------------------------------------------------------
    // control signals
    sc_signal<bool> pc_src;
    sc_signal<WORD> mem_data;
    // MEM stage ---------------------------------------------------------------

    // MEM/WB register
    sc_signal<WORD> mem_wb_mem_data_out;
    sc_signal<WORD> mem_wb_alu_result_out;
    sc_signal<WORD> mem_wb_st_mux_out;
    // control signals
    sc_signal<bool> mem_wb_mem_to_reg_out;
    sc_signal<bool> mem_wb_reg_write_out;

    // WB stage ----------------------------------------------------------------
    // wb mux
    sc_signal<WORD> wb_mux_out;
    // WB stage ----------------------------------------------------------------

    // ### COMPONENTS ###
    program_counter program_counter("program_counter");
    program_counter.clock(clock);                      // input
    program_counter.jump_address(if_mux_jump_address); // input
    program_counter.address(pc_address);               // output

    instruction_memory instruction_memory("instruction_memory");
    instruction_memory.address(pc_address); // input
    instruction_memory.inst(im_inst);       // output

    adder if_adder("if_adder");
    if_adder.a(pc_address); // input
    if_adder.b(if_adder_4); // input
    if_adder.s(if_adder_s); // output

    mux if_mux("if_mux");
    if_mux.a(if_adder_s);             // input
    if_mux.b(ex_mem_alu_adder_s_out); // input
    if_mux.sel(pc_src);               // input
    if_mux.out(if_mux_jump_address);  // output

    if_id_reg if_id_reg("if_id_reg");
    if_id_reg.clock(clock);             // input
    if_id_reg.inst_in(im_inst);         // input
    if_id_reg.inst_out(if_id_inst_out); // output

    register_file register_file("register_file");
    register_file.reg1(if_id_inst_out);            // input
    register_file.reg2(if_id_inst_out);            // input
    register_file.write_reg(if_id_inst_out);       // input
    register_file.write_data(wb_mux_out);          // input
    register_file.reg_write(mem_wb_reg_write_out); // input
    register_file.data1(rf_data1);                 // output
    register_file.data2(rf_data2);                 // output

    sign_extender sign_extender("sign_extender");
    sign_extender.input(if_id_inst_out); // input
    sign_extender.output(se_output);     // output

    id_ex_reg id_ex_reg("id_ex_reg");
    id_ex_reg.clock(clock);
    id_ex_reg.adder_in(if_adder_s);
    id_ex_reg.adder_out(id_ex_adder_out);

    id_ex_reg.data1_in(rf_data1);
    id_ex_reg.data2_in(rf_data2);
    id_ex_reg.data1_out(id_ex_data1_out);
    id_ex_reg.data2_out(id_ex_data2_out);
    id_ex_reg.inst_15_0_in(se_output);
    id_ex_reg.inst_20_16_in(if_id_inst_out);
    id_ex_reg.inst_15_11_in(if_id_inst_out);
    id_ex_reg.inst_15_0_out(id_ex_inst_15_0_out);
    id_ex_reg.inst_20_16_out(id_ex_inst_20_16_out);
    id_ex_reg.inst_15_11_out(id_ex_inst_15_11_out);

    // control signals
    // EX
    id_ex_reg.alu_op_in(alu_op);
    id_ex_reg.alu_op_out(id_ex_alu_op_out);

    id_ex_reg.alu_src_in(alu_src);
    id_ex_reg.alu_src_out(id_ex_alu_src_out);

    id_ex_reg.reg_dst_in(reg_dst);
    id_ex_reg.reg_dst_out(id_ex_reg_dst_out);
    // M
    id_ex_reg.branch_in(branch);
    id_ex_reg.branch_out(id_ex_branch_out);

    id_ex_reg.mem_write_in(mem_write);
    id_ex_reg.mem_write_out(id_ex_mem_write_out);

    id_ex_reg.mem_read_in(mem_read);
    id_ex_reg.mem_read_out(id_ex_mem_read_out);
    // WB
    id_ex_reg.mem_to_reg_in(mem_to_reg);
    id_ex_reg.mem_to_reg_out(id_ex_mem_to_reg_out);

    id_ex_reg.reg_write_in(reg_write);
    id_ex_reg.reg_write_out(id_ex_reg_write_out);

    shift_left_2 shift_left_2("shift_left_2");
    shift_left_2.in(id_ex_inst_15_0_out); // input
    shift_left_2.out(sl2_out);            // output

    adder ex_adder("ex_adder");
    ex_adder.a(id_ex_adder_out); // input
    ex_adder.b(sl2_out);         // input
    ex_adder.s(ex_adder_s);      // output

    mux ex_alu_mux("ex_alu_mux");
    ex_alu_mux.a(id_ex_data2_out);     // input
    ex_alu_mux.b(id_ex_inst_15_0_out); // input
    ex_alu_mux.sel(id_ex_alu_src_out); // input
    ex_alu_mux.out(ex_alu_mux_out);    // output

    alu alu("alu");
    alu.a(id_ex_data1_out);       // input
    alu.b(ex_alu_mux_out);        // input
    alu.alu_op(id_ex_alu_op_out); // input
    alu.result(alu_result);       // output
    alu.zero(alu_zero);           // output

    // sw mux reg (used for choosing the register to write on ST instruction)
    mux ex_st_mux("ex_st_mux");
    ex_st_mux.a(id_ex_inst_20_16_out); // input
    ex_st_mux.b(id_ex_inst_15_11_out); // input
    ex_st_mux.sel(id_ex_reg_dst_out);  // input
    ex_st_mux.out(ex_st_mux_out);      // output

    ex_mem_reg ex_mem_reg("ex_mem_reg");
    ex_mem_reg.clock(clock);
    ex_mem_reg.adder_in(ex_adder_s);
    ex_mem_reg.adder_out(ex_mem_adder_out);

    ex_mem_reg.alu_result_in(alu_result);
    ex_mem_reg.alu_result_out(ex_mem_alu_result_out);

    ex_mem_reg.data2_in(id_ex_data2_out);
    ex_mem_reg.data2_out(ex_mem_data2_out);

    ex_mem_reg.st_mux_in(ex_st_mux_out);
    ex_mem_reg.st_mux_out(ex_mem_st_mux_out);

    // control signals
    // M
    ex_mem_reg.alu_zero_in(alu_zero);
    ex_mem_reg.alu_zero_out(ex_mem_alu_zero_out);

    ex_mem_reg.branch_in(id_ex_branch_out);
    ex_mem_reg.branch_out(ex_mem_branch_out);

    ex_mem_reg.mem_write_in(id_ex_mem_write_out);
    ex_mem_reg.mem_write_out(ex_mem_mem_write_out);

    ex_mem_reg.mem_read_in(id_ex_mem_read_out);
    ex_mem_reg.mem_read_out(ex_mem_mem_read_out);
    // WB
    ex_mem_reg.mem_to_reg_in(id_ex_mem_to_reg_out);
    ex_mem_reg.mem_to_reg_out(ex_mem_mem_to_reg_out);

    ex_mem_reg.reg_write_in(id_ex_reg_write_out);
    ex_mem_reg.reg_write_out(ex_mem_reg_write_out);

    data_memory data_memory("data_memory");
    data_memory.address(ex_mem_alu_result_out);
    data_memory.write_data(ex_mem_data2_out);
    data_memory.data(mem_data);
    data_memory.mem_read(ex_mem_mem_read_out);
    data_memory.mem_write(ex_mem_mem_write_out);

    mem_wb_reg mem_wb_reg("mem_wb_reg");
    mem_wb_reg.clock(clock);
    mem_wb_reg.mem_data_in(mem_data);
    mem_wb_reg.mem_data_out(mem_wb_mem_data_out);

    mem_wb_reg.alu_result_in(ex_mem_alu_result_out);
    mem_wb_reg.alu_result_out(mem_wb_alu_result_out);

    mem_wb_reg.st_mux_in(ex_mem_st_mux_out);
    mem_wb_reg.st_mux_out(mem_wb_st_mux_out);

    // control signals
    // WB
    mem_wb_reg.mem_to_reg_in(ex_mem_mem_to_reg_out);
    mem_wb_reg.mem_to_reg_out(mem_wb_mem_to_reg_out);

    mem_wb_reg.reg_write_in(ex_mem_reg_write_out);
    mem_wb_reg.reg_write_out(mem_wb_reg_write_out);

    mux wb_mux("wb_mux");
    wb_mux.a(mem_wb_mem_data_out);
    wb_mux.b(mem_wb_alu_result_out);
    wb_mux.sel(mem_wb_mem_to_reg_out);
    wb_mux.out(wb_mux_out);

    // # READ DATA FILE AND LOAD INTO DATA MEMORY #
    // fstream data_file;
    // data_file.open("data.in");

    // // Checks if the file was found
    // if (!data_file) {
    //     cerr << "Error: file could not be opened" << endl;
    //     return 1;
    // }

    // // Loads all data into the memory
    // int dest = 0;
    // while (data_file >> dest) {
    //     data_file >> data_memory.memory[dest];
    // }
    // data_file.close();

    // # READ INSTRUCTION FILE AND LOAD INTO INSTRUCTION MEMORY #
    fstream inst_file;
    inst_file.open("instruction.in");

    // Checks if the file was found
    if (!inst_file) {
        cerr << "Error: file could not be opened." << endl;
        return 1;
    }

    map<string, int> opcode_from_string{
        // r-type
        {"and", 0b000000},
        {"or", 0b000001},
        {"xor", 0b000010},
        {"not", 0b000011},
        {"cmp", 0b000100},
        {"add", 0b000101},
        {"sub", 0b000110},
        // i-type
        {"ld", 0b000111},
        {"st", 0b001000},
        // j-type
        {"j", 0b001001},
        {"jn", 0b001010},
        {"jz", 0b001011},
    };

    // Loads all instructions into the memory
    string opcode_name = "";
    // int reg1, reg2, dest;
    int i = 0;
    while (inst_file >> opcode_name) {
        WORD instruction;

        bool rtype = false;
        bool itype = false;
        bool jtype = false;
        ALU_OP alu_op(-1);
        switch (opcode_from_string[opcode_name]) {
        case OP_AND: // r-type
            instruction = OP_AND;
            alu_op = AND;
            rtype = true;
            break;
        case OP_OR:
            instruction = OP_OR;
            alu_op = OR;
            rtype = true;
            break;
        case OP_XOR:
            instruction = OP_XOR;
            alu_op = XOR;
            rtype = true;
            break;
        case OP_NOT:
            instruction = OP_NOT;
            alu_op = NOT;
            rtype = true;
            break;
        case OP_CMP:
            instruction = OP_CMP;
            alu_op = CMP;
            rtype = true;
            break;
        case OP_ADD:
            instruction = OP_ADD;
            alu_op = ADD;
            rtype = true;
            break;
        case OP_SUB:
            instruction = OP_SUB;
            alu_op = SUB;
            rtype = true;
            break;
        case OP_LD: // i-type
            instruction = OP_LD;
            itype = true;
            break;
        case OP_ST:
            instruction = OP_ST;
            itype = true;
            break;
        case OP_J: // j-type
            instruction = OP_J;
            jtype = true;
        case OP_JN:
            instruction = OP_JN;
            jtype = true;
        case OP_JZ:
            instruction = OP_JZ;
            jtype = true;
        }

        if (rtype) { // r-type
            int rs;
            int rt;
            int rd;
            REG_ADDR shamt = 0;
            // convert alu_op to 6 bit vector, since that's the size of funct
            sc_bv<6> funct = alu_op;

            inst_file >> rs >> rt >> rd;

            instruction = (instruction, REG_ADDR(rs), REG_ADDR(rt),
                           REG_ADDR(rd), shamt, funct);
        } else if (itype) { // i-type
            int rs;
            int rt;
            int imm;

            inst_file >> rs >> rt >> imm;

            instruction = (instruction, REG_ADDR(rs), REG_ADDR(rt), IMM(imm));
        } else if (jtype) {
            int addr;

            inst_file >> addr;

            instruction = (instruction, sc_bv<26>(addr));
        }

        instruction_memory.memory[i++] = instruction;
    }
    inst_file.close();

    // ### TESTBENCH ###
    testbench testbench("testbench");
    testbench.clock(clock);

    // ### WAVEFORM ###
    sc_trace_file *fp;
    fp = sc_create_vcd_trace_file("wave");
    fp->set_time_unit(1, sc_core::SC_NS);

    sc_trace(fp, clock, "0-0-clock");

    sc_trace(fp, program_counter.jump_address,
             "program_counter|0-1-jump_address");
    sc_trace(fp, program_counter.address, "program_counter|0-2-address");

    sc_trace(fp, instruction_memory.address, "instruction_memory|0-1-address");
    sc_trace(fp, instruction_memory.inst, "instruction_memory|0-2-inst");

    sc_trace(fp, if_id_reg.inst_in, "if_id_reg|0-1-inst_in");
    sc_trace(fp, if_id_reg.inst_out, "if_id_reg|0-2-inst_out");

    sc_trace(fp, register_file.reg1, "register_file|0-1-reg1");
    sc_trace(fp, register_file.reg2, "register_file|0-2-reg2");
    sc_trace(fp, register_file.write_reg, "register_file|0-3-write_reg");
    sc_trace(fp, register_file.write_data, "register_file|0-4-write_data");
    sc_trace(fp, register_file.reg_write, "register_file|0-5-reg_write");
    sc_trace(fp, register_file.data1, "register_file|0-6-data1");
    sc_trace(fp, register_file.data2, "register_file|0-7-data2");

    sc_trace(fp, sign_extender.input, "sign_extender|0-1-input");
    sc_trace(fp, sign_extender.output, "sign_extender|0-2-output");

    sc_trace(fp, if_adder.a, "if_adder|0-1-a");
    sc_trace(fp, if_adder.b, "if_adder|0-2-b");
    sc_trace(fp, if_adder.s, "if_adder|0-3-s");

    sc_trace(fp, id_ex_reg.data1_in, "id_ex_reg|1-1-data1_in");
    sc_trace(fp, id_ex_reg.data2_in, "id_ex_reg|1-2-data2_in");
    sc_trace(fp, id_ex_reg.data1_out, "id_ex_reg|1-3-data1_out");
    sc_trace(fp, id_ex_reg.data2_out, "id_ex_reg|1-4-data2_out");
    sc_trace(fp, id_ex_reg.inst_15_0_in, "id_ex_reg|1-5-inst_15_0_in");
    sc_trace(fp, id_ex_reg.inst_20_16_in, "id_ex_reg|1-6-inst_20_16_in");
    sc_trace(fp, id_ex_reg.inst_15_11_in, "id_ex_reg|1-7-inst_15_11_in");
    sc_trace(fp, id_ex_reg.inst_15_0_out, "id_ex_reg|1-8-inst_15_0_out");
    sc_trace(fp, id_ex_reg.inst_20_16_out, "id_ex_reg|1-9-inst_20_16_out");
    sc_trace(fp, id_ex_reg.inst_15_11_out, "id_ex_reg|1-10-inst_15_11_out");
    sc_trace(fp, id_ex_reg.adder_in, "id_ex_reg|1-11-adder_in");
    sc_trace(fp, id_ex_reg.adder_out, "id_ex_reg|1-11-adder_out");

    sc_start();

    sc_close_vcd_trace_file(fp);

    return 0;
}
