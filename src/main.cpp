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
#include "modules/and.hpp"
#include "modules/control.hpp"
#include "modules/data_memory.hpp"
#include "modules/ex_mem_reg.hpp"
#include "modules/id_ex_reg.hpp"
#include "modules/if_id_reg.hpp"
#include "modules/instruction_memory.hpp"
#include "modules/mem_wb_reg.hpp"
#include "modules/mux.hpp"
#include "modules/mux3x1.hpp"
#include "modules/mux3x2.hpp"
#include "modules/reg_addr_mux.hpp"
#include "modules/program_counter.hpp"
#include "modules/register_file.hpp"
#include "modules/shift_left_2.hpp"
#include "modules/sign_extender.hpp"
#include "modules/fowarding_unity.hpp"
#include "modules/detection_unity.hpp"

SC_MODULE(testbench) {
    sc_in_clk clock;

    void test() {
        for (size_t i = 0; i < 100; i++) {
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
    sc_signal<bool> pc_write;
    // instruction memory
    sc_signal<WORD> im_inst;
    // IF stage adder
    sc_signal<WORD> if_adder_1{"constant_1", WORD(1)};
    sc_signal<WORD> if_adder_s;
    // IF stage ----------------------------------------------------------------

    // IF/ID register
    sc_signal<WORD> if_id_inst_out;
    sc_signal<WORD> if_id_address_out;
    sc_signal<WORD> if_id_adder_s_out;
    sc_signal<bool> if_id_write;

    // ID stage ----------------------------------------------------------------
    // register file
    sc_signal<WORD> rf_data1;
    sc_signal<WORD> rf_data2;
    // sign extender
    sc_signal<WORD> se_output;
    // control signals
    // EX
    sc_signal<bool> jump;
    sc_signal<ALU_OP> alu_op;
    sc_signal<bool> alu_src;
    sc_signal<bool> reg_dst;
    // M
    sc_signal<bool> branch;
    sc_signal<bool> mem_write;
    sc_signal<bool> mem_read;

    sc_signal<bool> mem_write_mux_out;
    sc_signal<bool> reg_write_mux_out;
    sc_signal<bool> mem_read_mux_out;

    // WB
    sc_signal<bool> mem_to_reg;
    sc_signal<bool> reg_write;
    // ID stage ----------------------------------------------------------------

    // ID/EX register
    sc_signal<WORD> id_ex_adder_out;
    sc_signal<WORD> id_ex_data1_out;
    sc_signal<WORD> id_ex_data2_out;
    sc_signal<WORD> id_ex_inst_15_0_out;
    sc_signal<REG_ADDR> id_ex_inst_20_16_out;
    sc_signal<REG_ADDR> id_ex_inst_15_11_out;
    sc_signal<REG_ADDR> id_ex_inst_25_21_out;
    // control signals
    // EX
    sc_signal<ALU_OP> id_ex_alu_op_out;
    sc_signal<bool> id_ex_alu_src_out;
    sc_signal<bool> id_ex_reg_dst_out;
    sc_signal<bool> id_ex_jump_out;
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

    // Jump mux
    sc_signal<WORD> ex_jump_mux_out;
    // alu
    sc_signal<WORD> alu_result;
    sc_signal<bool> alu_zero;
    // alu mux
    sc_signal<WORD> ex_alu_mux_out;
    // sw mux reg (used for choosing the register to write on ST instruction)
    sc_signal<REG_ADDR> ex_st_mux_out;
    // shift left 2
    sc_signal<WORD> sl2_out;
    // EX stage ----------------------------------------------------------------

    // EX/MEM register
    sc_signal<WORD> ex_mem_alu_adder_s_out;
    sc_signal<WORD> ex_mem_adder_out;
    sc_signal<WORD> ex_mem_alu_result_out;
    sc_signal<REG_ADDR> ex_mem_st_mux_out;
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
    sc_signal<REG_ADDR> mem_wb_st_mux_out;
    // control signals
    sc_signal<bool> mem_wb_mem_to_reg_out;
    sc_signal<bool> mem_wb_reg_write_out;

    // WB stage ----------------------------------------------------------------
    // wb mux
    sc_signal<WORD> wb_mux_out;
    // WB stage ----------------------------------------------------------------

    // HAZARD
    sc_signal<int> ex_upper_alu_mux, ex_lower_alu_mux;

    // ### COMPONENTS ###
    program_counter program_counter("program_counter");
    program_counter.clock(clock);                      // input
    program_counter.jump_address(if_mux_jump_address); // input
    program_counter.pc_write(pc_write);                 // input
    program_counter.address(pc_address);               // output

    instruction_memory instruction_memory("instruction_memory");
    instruction_memory.address(pc_address); // input
    instruction_memory.inst(im_inst);       // output

    adder if_adder("if_adder");
    if_adder.a(pc_address); // input
    if_adder.b(if_adder_1); // input
    if_adder.s(if_adder_s); // output

    mux if_mux("if_mux");
    if_mux.a(if_adder_s);             // input
    //if_mux.b(ex_mem_alu_adder_s_out); // input
    if_mux.b(ex_mem_adder_out); // input
    if_mux.sel(pc_src);               // input
    if_mux.out(if_mux_jump_address);  // output

    if_id_reg if_id_reg("if_id_reg");
    if_id_reg.clock(clock);             // input
    if_id_reg.inst_in(im_inst);         // input
    if_id_reg.if_adder_s_in(if_adder_s); // input 
    if_id_reg.if_id_write(if_id_write); // input 
    if_id_reg.inst_out(if_id_inst_out); // output
    if_id_reg.if_adder_s_out(if_id_adder_s_out); // output

    control control("control");
    control.instruction(if_id_inst_out);
    // EX
    control.alu_op(alu_op);
    control.alu_src(alu_src);
    control.reg_dst(reg_dst);
    // M
    control.mem_write(mem_write);
    control.mem_read(mem_read);
    // WB
    control.reg_write(reg_write);
    control.mem_to_reg(mem_to_reg);
    control.branch(branch);
    control.jump(jump);

    register_file register_file("register_file");
    register_file.reg1(if_id_inst_out);            // input
    register_file.reg2(if_id_inst_out);            // input
    register_file.write_reg(mem_wb_st_mux_out);    // input
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
    id_ex_reg.inst_25_21_out(id_ex_inst_25_21_out);

    // control signals
    // EX
    id_ex_reg.alu_op_in(alu_op);
    id_ex_reg.alu_op_out(id_ex_alu_op_out);

    id_ex_reg.alu_src_in(alu_src);
    id_ex_reg.alu_src_out(id_ex_alu_src_out);

    id_ex_reg.reg_dst_in(reg_dst);
    id_ex_reg.reg_dst_out(id_ex_reg_dst_out);

    id_ex_reg.jump_in(jump);
    id_ex_reg.jump_out(id_ex_jump_out);
    // M
    id_ex_reg.branch_in(branch);
    id_ex_reg.branch_out(id_ex_branch_out);

    id_ex_reg.mem_write_in(mem_write_mux_out);
    id_ex_reg.mem_write_out(id_ex_mem_write_out);

    id_ex_reg.mem_read_in(mem_read);
    id_ex_reg.mem_read_out(id_ex_mem_read_out);
    // WB
    id_ex_reg.mem_to_reg_in(mem_to_reg);
    id_ex_reg.mem_to_reg_out(id_ex_mem_to_reg_out);

    id_ex_reg.reg_write_in(reg_write_mux_out);
    id_ex_reg.reg_write_out(id_ex_reg_write_out);

    // NOTE: Don't need to multiply the PC address
    //shift_left_2 shift_left_2("shift_left_2");
    //shift_left_2.in(id_ex_inst_15_0_out); // input
    //shift_left_2.out(sl2_out);            // output

    adder ex_adder("ex_adder");
    ex_adder.a(id_ex_adder_out); // input
    ex_adder.b(id_ex_inst_15_0_out); // input
    ex_adder.s(ex_adder_s);      // output

    mux ex_jump_mux("ex_jump_mux");
    ex_jump_mux.a(ex_adder_s);     // input
    ex_jump_mux.b(id_ex_inst_15_0_out); // input
    ex_jump_mux.sel(id_ex_jump_out); // input
    ex_jump_mux.out(ex_jump_mux_out);    // output

    sc_signal<WORD> ex_mux2mux3x1;
    mux ex_alu_mux("ex_alu_mux");
    ex_alu_mux.a(id_ex_data2_out);     // input
    ex_alu_mux.b(id_ex_inst_15_0_out); // input
    ex_alu_mux.sel(id_ex_alu_src_out); // input
    ex_alu_mux.out(ex_mux2mux3x1);    // output
    
    mux3x1 ex_alu_lower_mux3x1("ex_alu_lower_mux3x1");
    ex_alu_lower_mux3x1.a(ex_mux2mux3x1);     // input
    ex_alu_lower_mux3x1.b(ex_mem_alu_result_out); // input
    ex_alu_lower_mux3x1.c(wb_mux_out); // input
    ex_alu_lower_mux3x1.sel(ex_lower_alu_mux); // input
    ex_alu_lower_mux3x1.out(ex_alu_mux_out);    // output
    

    sc_signal<WORD> ex_upper_mux_alu;
    mux3x1 ex_alu_upper_mux3x1("ex_alu_upper_mux3x1");
    ex_alu_upper_mux3x1.a(id_ex_data1_out);     // input
    ex_alu_upper_mux3x1.b(wb_mux_out); // input
    ex_alu_upper_mux3x1.c(ex_mem_alu_result_out); // input
    ex_alu_upper_mux3x1.sel(ex_upper_alu_mux); // input
    ex_alu_upper_mux3x1.out(ex_upper_mux_alu);    // output

    alu alu("alu");
    alu.a(ex_upper_mux_alu);       // input
    alu.b(ex_alu_mux_out);        // input
    alu.alu_op(id_ex_alu_op_out); // input
    alu.result(alu_result);       // output
    alu.zero(alu_zero);           // output

    // sw mux reg (used for choosing the register to write on ST instruction)
    reg_addr_mux ex_st_mux("ex_st_mux");
    ex_st_mux.a(id_ex_inst_20_16_out); // input
    ex_st_mux.b(id_ex_inst_15_11_out); // input
    ex_st_mux.sel(id_ex_reg_dst_out);  // input
    ex_st_mux.out(ex_st_mux_out);      // output

    ex_mem_reg ex_mem_reg("ex_mem_reg");
    ex_mem_reg.clock(clock);
    // TODO: change the name
    ex_mem_reg.adder_in(ex_jump_mux_out);
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

    and_gate and_gate("and_gate");
    and_gate.a(ex_mem_branch_out);
    and_gate.b(ex_mem_alu_zero_out);
    and_gate.out(pc_src);

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

    // ### HAZARDS ###
    fowarding_unity fowarding_unity("fowarding_unity");
    fowarding_unity.ex_rs_in(id_ex_inst_25_21_out);
    fowarding_unity.ex_rt_in(id_ex_inst_20_16_out);
    fowarding_unity.mem_rd_in(ex_mem_st_mux_out);
    fowarding_unity.wb_rd_in(mem_wb_st_mux_out);

    fowarding_unity.mem_reg_write(ex_mem_mem_to_reg_out);
    fowarding_unity.wb_reg_write(mem_wb_mem_to_reg_out);

    fowarding_unity.ex_upper_alu_mux(ex_upper_alu_mux);
    fowarding_unity.ex_lower_alu_mux(ex_lower_alu_mux);

    sc_signal<bool> control_mux_nop;

    detection_unity detection_unity("detection_unity");
    // # Input #
    detection_unity.if_id_reg_rs(if_id_inst_out);
    detection_unity.if_id_reg_rt(if_id_inst_out);
    detection_unity.id_ex_reg_rt(id_ex_inst_15_0_out);

    // Control
    detection_unity.id_ex_mem_read(id_ex_mem_read_out);
    
    // # Output #
    // Control
    detection_unity.control_mux_nop(control_mux_nop);
    detection_unity.pc_write(pc_write);
    detection_unity.if_id_write(if_id_write);

    sc_signal<bool> control_mux_false{"constant_false", false};

    mux3x2 control_mux3x2("control_mux3x2");
    control_mux3x2.a(mem_write);
    control_mux3x2.b(reg_write);
    control_mux3x2.c(control_mux_false);
    control_mux3x2.sel(control_mux_nop);
    control_mux3x2.out_a(mem_write_mux_out);
    control_mux3x2.out_b(reg_write_mux_out);


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
        {"and", OP_AND},
        {"or", OP_OR},
        {"xor", OP_XOR},
        {"not", OP_NOT},
        {"cmp", OP_CMP},
        {"add", OP_ADD},
        {"sub", OP_SUB},
        // i-type
        {"ld", OP_LD},
        {"st", OP_ST},
        // j-type
        {"j", OP_J},
        {"jn", OP_JN},
        {"jz", OP_JZ},
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
            break;
        case OP_JN:
            instruction = OP_JN;
            jtype = true;
            break;
        case OP_JZ:
            instruction = OP_JZ;
            jtype = true;
            break;
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
            int field[3];

            inst_file >> field[0] >> field[1] >> field[2];

            instruction = (instruction, REG_ADDR(field[0]), 
                REG_ADDR(field[1]), IMM(field[2]));
        }

        std::cout << instruction << std::endl;
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
    sc_trace(fp, if_id_reg.if_adder_s_in, "if_id_reg|1-3-if_adder_s_in"); 
    sc_trace(fp, if_id_reg.if_adder_s_out, "if_id_reg|1-4-if_adder_s_out");

    sc_trace(fp, register_file.reg1, "register_file|0-1-reg1");
    sc_trace(fp, register_file.reg2, "register_file|0-2-reg2");
    sc_trace(fp, register_file.write_reg, "register_file|0-3-write_reg");
    sc_trace(fp, register_file.write_data, "register_file|0-4-write_data");
    sc_trace(fp, register_file.reg_write, "register_file|0-5-reg_write");
    sc_trace(fp, register_file.data1, "register_file|0-6-data1");
    sc_trace(fp, register_file.data2, "register_file|0-7-data2");
    for (int i = 0; i < 9; i++) {
        sc_trace(fp, register_file.registers[i], "register_file|0-7-registers-" + std::to_string(i));
    }

    sc_trace(fp, sign_extender.input, "sign_extender|0-1-input");
    sc_trace(fp, sign_extender.output, "sign_extender|0-2-output");

    sc_trace(fp, if_adder.a, "if_adder|0-1-a");
    sc_trace(fp, if_adder.b, "if_adder|0-2-b");
    sc_trace(fp, if_adder.s, "if_adder|0-3-s");

    sc_trace(fp, id_ex_reg.data1_in,  "id_ex_reg|1-1-data1_in");
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
    sc_trace(fp, id_ex_reg.adder_out, "id_ex_reg|1-12-adder_out");

    sc_trace(fp, id_ex_reg.alu_op_in,       "id_ex_reg|ctrl|1-13-alu_op_in");
    sc_trace(fp, id_ex_reg.alu_op_out,      "id_ex_reg|ctrl|1-14alu_op_out");
    sc_trace(fp, id_ex_reg.alu_src_in,      "id_ex_reg|ctrl|1-15-alu_src_in");
    sc_trace(fp, id_ex_reg.alu_src_out,     "id_ex_reg|ctrl|1-16-alu_src_out");
    sc_trace(fp, id_ex_reg.reg_dst_in,      "id_ex_reg|ctrl|1-17-reg_dst_in");
    sc_trace(fp, id_ex_reg.reg_dst_out,     "id_ex_reg|ctrl|1-18-reg_dst_out");
    sc_trace(fp, id_ex_reg.branch_in,       "id_ex_reg|ctrl|1-19-branch_in");
    sc_trace(fp, id_ex_reg.branch_out,      "id_ex_reg|ctrl|1-20-branch_out");
    sc_trace(fp, id_ex_reg.mem_write_in,    "id_ex_reg|ctrl|1-21-mem_write_in");
    sc_trace(fp, id_ex_reg.mem_write_out,   "id_ex_reg|ctrl|1-22-mem_write_out");
    sc_trace(fp, id_ex_reg.mem_read_in,     "id_ex_reg|ctrl|1-23-mem_read_in");
    sc_trace(fp, id_ex_reg.mem_read_out,    "id_ex_reg|ctrl|1-24-mem_read_out");
    sc_trace(fp, id_ex_reg.mem_to_reg_in,   "id_ex_reg|ctrl|1-25-mem_to_reg_in");
    sc_trace(fp, id_ex_reg.mem_to_reg_out,  "id_ex_reg|ctrl|1-26-mem_to_reg_out");
    sc_trace(fp, id_ex_reg.reg_write_in,    "id_ex_reg|ctrl|1-27-reg_write_in");
    sc_trace(fp, id_ex_reg.reg_write_out,   "id_ex_reg|ctrl|1-28-reg_write_out");

    sc_trace(fp, data_memory.address,       "data_memory|1-0-address");
    sc_trace(fp, data_memory.write_data,    "data_memory|1-1-write_data");
    sc_trace(fp, data_memory.data,          "data_memory|1-2-data");
    for (int i = 0; i < 9; i++) {
        sc_trace(fp, data_memory.memory[i],         "data_memory|1-3-memory-" + std::to_string(i));
    }
    sc_trace(fp, data_memory.mem_read,      "data_memory|crtl|1-0-mem_read");
    sc_trace(fp, data_memory.mem_write,     "data_memory|crtl|1-1-mem_write");

    sc_trace(fp, wb_mux.a, "wb_mux|1-2-a");
    sc_trace(fp, wb_mux.b, "wb_mux|1-2-b");
    sc_trace(fp, wb_mux.sel, "wb_mux|1-2-sel");
    sc_trace(fp, wb_mux.out, "wb_mux|1-2-out");

    sc_trace(fp, if_mux.a, "if_mux|1-2-a");             // input
    sc_trace(fp, if_mux.b, "if_mux|1-2-b"); // input
    sc_trace(fp, if_mux.sel, "if_mux|1-2-sel");               // input
    sc_trace(fp, if_mux.out, "if_mux|1-2-out");  // output

    sc_trace(fp, ex_alu_mux.a,      "ex_alu_mux|1-0-a");     
    sc_trace(fp, ex_alu_mux.b,      "ex_alu_mux|1-1-b"); 
    sc_trace(fp, ex_alu_mux.sel,    "ex_alu_mux|1-2-sel"); 
    sc_trace(fp, ex_alu_mux.out,    "ex_alu_mux|1-2-out");    

    sc_trace(fp, alu.a,         "alu|1-0-a");       
    sc_trace(fp, alu.b,         "alu|1-1-b");        
    sc_trace(fp, alu.alu_op,    "alu|1-2-alu_op"); 
    sc_trace(fp, alu.result,    "alu|1-3-result");       
    sc_trace(fp, alu.zero,      "alu|1-4-zero");           

    sc_trace(fp, control.instruction,   "control|0-instruction");
    sc_trace(fp, control.alu_op,        "control|1-alu_op");
    sc_trace(fp, control.alu_src,       "control|2-alu_src");
    sc_trace(fp, control.reg_dst,       "control|3-reg_dst");
    sc_trace(fp, control.mem_write,     "control|4-mem_write");
    sc_trace(fp, control.mem_read,      "control|5-mem_read");
    sc_trace(fp, control.reg_write,     "control|6-reg_write");
    sc_trace(fp, control.mem_to_reg,    "control|7-mem_to_reg");
    sc_trace(fp, control.branch,        "control|7-branch");
    sc_trace(fp, control.jump,        "control|8-jump");

    sc_trace(fp, ex_adder.a, "ex_adder|1-0-a");
    sc_trace(fp, ex_adder.b, "ex_adder|1-1-b");
    sc_trace(fp, ex_adder.s, "ex_adder|1-2-s");

    sc_trace(fp, ex_mem_reg.adder_in,       "ex_mem_reg|1-0-adder_in");
    sc_trace(fp, ex_mem_reg.adder_out,      "ex_mem_reg|1-1-adder_out");
    sc_trace(fp, ex_mem_reg.alu_result_in,  "ex_mem_reg|1-2-alu_result_in");
    sc_trace(fp, ex_mem_reg.alu_result_out, "ex_mem_reg|1-3-alu_result_out");
    sc_trace(fp, ex_mem_reg.data2_in,       "ex_mem_reg|1-4-data2_in");
    sc_trace(fp, ex_mem_reg.data2_out,      "ex_mem_reg|1-5-data2_out");
    sc_trace(fp, ex_mem_reg.st_mux_in,      "ex_mem_reg|1-6-st_mux_in");
    sc_trace(fp, ex_mem_reg.st_mux_out,     "ex_mem_reg|1-7-st_mux_out");

    sc_trace(fp, ex_mem_reg.alu_zero_in,    "ex_mem_reg|ctrl|1-0-alu_zero_in");
    sc_trace(fp, ex_mem_reg.alu_zero_out,   "ex_mem_reg|ctrl|1-1-alu_zero_out");
    sc_trace(fp, ex_mem_reg.branch_in,      "ex_mem_reg|ctrl|1-2-branch_in");
    sc_trace(fp, ex_mem_reg.branch_out,     "ex_mem_reg|ctrl|1-3-branch_out");
    sc_trace(fp, ex_mem_reg.mem_write_in,   "ex_mem_reg|ctrl|1-4-mem_write_in");
    sc_trace(fp, ex_mem_reg.mem_write_out,  "ex_mem_reg|ctrl|1-5-mem_write_out");
    sc_trace(fp, ex_mem_reg.mem_read_in,    "ex_mem_reg|ctrl|1-6-mem_read_in");
    sc_trace(fp, ex_mem_reg.mem_read_out,   "ex_mem_reg|ctrl|1-7-mem_read_out");
    sc_trace(fp, ex_mem_reg.mem_to_reg_in,  "ex_mem_reg|ctrl|1-8-mem_to_reg_in");
    sc_trace(fp, ex_mem_reg.mem_to_reg_out, "ex_mem_reg|ctrl|1-9-mem_to_reg_out");
    sc_trace(fp, ex_mem_reg.reg_write_in,   "ex_mem_reg|ctrl|1-10-reg_write_in");
    sc_trace(fp, ex_mem_reg.reg_write_out,  "ex_mem_reg|ctrl|1-11-reg_write_out");

    sc_trace(fp, mem_wb_reg.mem_data_in,    "mem_wb_reg|1-0-mem_data_in");
    sc_trace(fp, mem_wb_reg.mem_data_out,   "mem_wb_reg|1-1-mem_data_out");
    sc_trace(fp, mem_wb_reg.alu_result_in,  "mem_wb_reg|1-2-alu_result_in");
    sc_trace(fp, mem_wb_reg.alu_result_out, "mem_wb_reg|1-3-alu_result_out");
    sc_trace(fp, mem_wb_reg.st_mux_in,      "mem_wb_reg|1-4-st_mux_in");
    sc_trace(fp, mem_wb_reg.st_mux_out,     "mem_wb_reg|1-5-st_mux_out");
    sc_trace(fp, mem_wb_reg.mem_to_reg_in,  "mem_wb_reg|ctrl|1-0-mem_to_reg_in");
    sc_trace(fp, mem_wb_reg.mem_to_reg_out, "mem_wb_reg|ctrl|1-1-mem_to_reg_out");
    sc_trace(fp, mem_wb_reg.reg_write_in,   "mem_wb_reg|ctrl|1-2-reg_write_in");
    sc_trace(fp, mem_wb_reg.reg_write_out,  "mem_wb_reg|ctrl|1-3-reg_write_out");

    sc_trace(fp, ex_st_mux.a,   "ex_st_mux|1-0-a");
    sc_trace(fp, ex_st_mux.b,   "ex_st_mux|1-1-b");
    sc_trace(fp, ex_st_mux.sel, "ex_st_mux|1-2-sel");
    sc_trace(fp, ex_st_mux.out, "ex_st_mux|1-3-out");


    // ### J-TYPE INST CHECK ###
    // Adder
    sc_trace(fp, ex_adder.a,        "J-TYPE|ex_adder|0-a");
    sc_trace(fp, ex_adder.b,        "J-TYPE|ex_adder|1-b");
    sc_trace(fp, ex_adder.s,        "J-TYPE|ex_adder|2-s");
    // Mux
    sc_trace(fp, if_mux.a,          "J-TYPE|if_mux|0-a");
    sc_trace(fp, if_mux.b,          "J-TYPE|if_mux|1-b");
    sc_trace(fp, if_mux.sel,        "J-TYPE|if_mux|2-sel");
    sc_trace(fp, if_mux.out,        "J-TYPE|if_mux|3-out");
    // Alu
    sc_trace(fp, alu.a,             "J-TYPE|alu|0-a");       
    sc_trace(fp, alu.b,             "J-TYPE|alu|1-b");        
    sc_trace(fp, alu.alu_op,        "J-TYPE|alu|2-alu_op"); 
    sc_trace(fp, alu.result,        "J-TYPE|alu|3-result");       
    sc_trace(fp, alu.zero,          "J-TYPE|alu|4-zero");           

    sc_trace(fp, ex_jump_mux.a,     "ex_jump_mux|0-a");     // input
    sc_trace(fp, ex_jump_mux.b,     "ex_jump_mux|1-b"); // input
    sc_trace(fp, ex_jump_mux.sel,   "ex_jump_mux|2-sel"); // input
    sc_trace(fp, ex_jump_mux.out,   "ex_jump_mux|3-out");    // output

    // ## HAZARD ##
    // # foward_unity #
    sc_trace(fp, fowarding_unity.ex_rs_in,          "HAZARD|fowarding_unity|0-ex_rs_in");
    sc_trace(fp, fowarding_unity.ex_rt_in,          "HAZARD|fowarding_unity|1-ex_rt_in");
    sc_trace(fp, fowarding_unity.mem_rd_in,         "HAZARD|fowarding_unity|2-mem_rd_in");
    sc_trace(fp, fowarding_unity.wb_rd_in,          "HAZARD|fowarding_unity|3-wb_rd_in");
    sc_trace(fp, fowarding_unity.mem_reg_write,     "HAZARD|fowarding_unity|4-mem_reg_write");
    sc_trace(fp, fowarding_unity.wb_reg_write,      "HAZARD|fowarding_unity|5-wb_reg_write");
    sc_trace(fp, fowarding_unity.ex_upper_alu_mux,  "HAZARD|fowarding_unity|6-ex_upper_alu_mux");
    sc_trace(fp, fowarding_unity.ex_lower_alu_mux,  "HAZARD|fowarding_unity|7-ex_lower_alu_mux");


    // # detection_unity #
    sc_trace(fp, detection_unity.if_id_reg_rs,      "HAZARD|detection_unity|0-if_id_reg_rs");
    sc_trace(fp, detection_unity.if_id_reg_rt,      "HAZARD|detection_unity|1-if_id_reg_rt");
    sc_trace(fp, detection_unity.id_ex_reg_rt,      "HAZARD|detection_unity|2-id_ex_reg_rt");
    sc_trace(fp, detection_unity.id_ex_mem_read,    "HAZARD|detection_unity|3-id_ex_mem_read");
    sc_trace(fp, detection_unity.control_mux_nop,   "HAZARD|detection_unity|4-control_mux_nop");
    sc_trace(fp, detection_unity.pc_write,          "HAZARD|detection_unity|5-pc_write");
    sc_trace(fp, detection_unity.if_id_write,       "HAZARD|detection_unity|6-if_id_write");

    // # control_mux3x2 #
    sc_trace(fp, control_mux3x2.a,      "HAZARD|control_mux3x2|0-a");
    sc_trace(fp, control_mux3x2.b,      "HAZARD|control_mux3x2|1-b");
    sc_trace(fp, control_mux3x2.c,      "HAZARD|control_mux3x2|2-c");
    sc_trace(fp, control_mux3x2.sel,    "HAZARD|control_mux3x2|3-sel");
    sc_trace(fp, control_mux3x2.out_a,  "HAZARD|control_mux3x2|4-out_a");
    sc_trace(fp, control_mux3x2.out_b,  "HAZARD|control_mux3x2|5-out_b");

    sc_start();

    sc_close_vcd_trace_file(fp);

    return 0;
}
