#include "systemc.h"

#include <bitset>
#include <fstream>
#include <string>

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

// INCLUDE MODULES
#include "definitions.hpp"
#include "modules/adder.hpp"
#include "modules/if_id_reg.hpp"
#include "modules/instruction_memory.hpp"
#include "modules/mux.hpp"
// #include "modules/alu.hpp"
#include "modules/program_counter.hpp"
#include "modules/register_file.hpp"
#include "modules/sign_extender.hpp"
// #include "modules/data_memory.hpp"

int sc_main(int argc, char *argv[]) {
    // ### CLOCK ###
    sc_clock clock("clock", 100, SC_NS, 0.5);

    // ### SIGNALS (WIRES) ###
    // IF stage ----------------------------------------------------------------
    // IF stage mux
    sc_signal<WORD> ifmux_jump_address;
    // program counter
    sc_signal<WORD> pc_address;
    // instruction memory
    sc_signal<WORD> im_inst;
    // IF stage adder
    sc_signal<WORD> if_adder_4{"constant 4", WORD(4)};
    sc_signal<WORD> if_adder_s;
    // IF stage ----------------------------------------------------------------

    // IF/ID register
    sc_signal<WORD> ifid_inst_out;
    sc_signal<WORD> ifid_address_out;

    // ID stage ----------------------------------------------------------------
    // register file
    sc_signal<WORD> rf_data1, rf_data2;
    // sign extender
    sc_signal<WORD> se_output;
    // ID stage ----------------------------------------------------------------

    // WB stage ----------------------------------------------------------------
    sc_signal<WORD> memwb_write_reg_out;
    sc_signal<bool> memwb_reg_write_out;
    // WB stage ----------------------------------------------------------------

    // ### COMPONENTS ###
    program_counter program_counter("program_counter");
    program_counter.clock(clock);                     // input
    program_counter.jump_address(ifmux_jump_address); // input
    program_counter.address(pc_address);              // output

    instruction_memory instruction_memory("instruction_memory");
    instruction_memory.address(pc_address); // input
    instruction_memory.inst(im_inst);       // output

    adder if_adder("if_adder");
    if_adder.a(pc_address);        // input
    if_adder.b(if_adder_4);        // input
    if_adder.s(if_adder_s);        // output

    // mux if_mux("if_mux");
    // // if_mux.a();
    // // if_mux.b();
    // // if_mux.sel();
    // if_mux.out(ifmux_jump_address); // output

    if_id_reg if_id_reg("if_id_reg");
    if_id_reg.clock(clock);            // input
    if_id_reg.inst_in(im_inst);        // input
    if_id_reg.inst_out(ifid_inst_out); // output

    register_file register_file("register_file");
    register_file.reg1(ifid_inst_out);             // input
    register_file.reg2(ifid_inst_out);             // input
    register_file.write_reg(ifid_inst_out);        // input
    register_file.write_data(memwb_write_reg_out); // input
    register_file.reg_write(memwb_reg_write_out);  // input
    register_file.data1(rf_data1);                 // output
    register_file.data2(rf_data2);                 // output

    sign_extender sign_extender("sign_extender");
    sign_extender.input(ifid_inst_out); // input
    sign_extender.output(se_output);    // output

    // # READ DATA FILE AND LOAD INTO DATA MEMORY #
    fstream instFs;
    //
    //    instFs.open("data.in");
    //
    //    // Checks if the file was found
    //    if(!instFs) {
    //        cerr << "Error: file could not be opened" << endl;
    //        return 1;
    //    }
    //
    //    // Loads all instructions into the memory
    //    int dest = 0;
    //    while (instFs >> dest) {
    //        instFs >> DataMemory.memory[dest];
    //    }
    //    instFs.close();
    //
    //  # READ INSTRUCTION FILE AND LOAD INTO INSTRUCTION MEMORY #
    instFs.open("instruction.in");

    // Checks if the file was found
    if (!instFs) {
        cerr << "Error: file could not be opened" << endl;
        return 1;
    }

    // Loads all instructions into the memory
    string opcode_name = "";
    int reg1, reg2, dest;
    WORD inst;

    int i = 0;
    // Checks if there is a instruction opCode in the line
    while (instFs >> opcode_name >> reg1 >> reg2 >> dest) {
        string instruction = "";

        // convert opcode string to int and save
        if (opcode_name == "AND")
            instruction += OP_AND;
        else if (opcode_name == "OR")
            instruction += OP_OR;
        else if (opcode_name == "XOR")
            instruction += OP_XOR;
        else if (opcode_name == "NOT")
            instruction += OP_NOT;
        else if (opcode_name == "CMP")
            instruction += OP_CMP;
        else if (opcode_name == "ADD")
            instruction += OP_ADD;
        else if (opcode_name == "SUB")
            instruction += OP_SUB;

        instruction += bitset<5>(reg1).to_string();
        instruction += bitset<5>(reg2).to_string();
        instruction += bitset<5>(dest).to_string();
        instruction += bitset<11>(0).to_string();

        // assign the binary string to the bit-vector
        for (int j = 0; j < (int)instruction.size(); ++j) {
            inst[31 - j] = instruction[j];
        }

        instruction_memory.memory[i] = inst;
        i++;
    }
    instFs.close();

    // ### TESTBENCH ###
    testbench testbench("testbench");
    testbench.clock(clock);

    // ### WAVEFORM ###
    sc_trace_file *fp;
    fp = sc_create_vcd_trace_file("wave");
    fp->set_time_unit(1, sc_core::SC_NS);

    sc_trace(fp, clock, "0-0-clock");

    //  ## MEMORY ##
    // sc_trace(fp, DataMemory.memory[0], "DataMemory|0-memory");
    // sc_trace(fp, DataMemory.memory[1], "DataMemory|1-memory");
    // sc_trace(fp, DataMemory.memory[2], "DataMemory|2-memory");
    // sc_trace(fp, DataMemory.memory[3], "DataMemory|3-memory");
    // sc_trace(fp, DataMemory.memory[4], "DataMemory|4-memory");
    // sc_trace(fp, DataMemory.memory[5], "DataMemory|5-memory");
    // sc_trace(fp, DataMemory.memory[6], "DataMemory|6-memory");
    // sc_trace(fp, DataMemory.memory[7], "DataMemory|7-memory");

    ////  ## GIGDRegister $$
    // sc_trace(fp, GIGDRegister.opCodeIn, "GIGDRegister|1-1-opCodeIGI");
    // sc_trace(fp, GIGDRegister.regRead1In, "GIGDRegister|1-2-regRead1IGI");
    // sc_trace(fp, GIGDRegister.regRead2In, "GIGDRegister|1-3-regRead2IGI");
    // sc_trace(fp, GIGDRegister.regWriteIn, "GIGDRegister|1-4-regWriteIGI");

    // sc_trace(fp, GIGDRegister.opCodeOut, "GIGDRegister|2-1-opCodeGIGD");
    // sc_trace(fp, GIGDRegister.regRead1Out, "GIGDRegister|2-2-regRead1GID");
    // sc_trace(fp, GIGDRegister.regRead2Out, "GIGDRegister|2-3-regRead2GID");
    // sc_trace(fp, GIGDRegister.regWriteOut, "GIGDRegister|2-4-regWriteGIGD");

    //// sc_trace(fp, GDEXRegister.opCodeIn,     "2-5-opCodeGIGD");
    //// sc_trace(fp, GDEXRegister.dataRead1In,  "2-6-dataRead1DGD");
    //// sc_trace(fp, GDEXRegister.dataRead2In,  "2-7-dataRead2DGD");
    //// sc_trace(fp, GDEXRegister.dataWriteIn,  "2-8-dataWriteAGD");

    // sc_trace(fp, GDEXRegister.opCodeOut, "GDEXRegister|3-1-opCodeGDA");
    // sc_trace(fp, GDEXRegister.dataRead1Out, "GDEXRegister|3-2-dataRead1GDA");
    // sc_trace(fp, GDEXRegister.dataRead2Out, "GDEXRegister|3-3-dataRead2GDA");
    // sc_trace(fp, GDEXRegister.regWriteOut, "GDEXRegister|3-4-regWriteGDD");
    // sc_trace(fp, GDEXRegister.dataWriteOut, "GDEXRegister|3-4-dataWriteGDD");
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

    // sc_trace(fp, adder.a, "adder|0-1-a");
    // sc_trace(fp, adder.b, "adder|0-2-b");
    // sc_trace(fp, adder.s, "adder|0-3-s");
    sc_start();

    sc_close_vcd_trace_file(fp);

    return 0;
}
