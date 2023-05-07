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
//#include "modules/alu.hpp"
#include "modules/program_counter.hpp"
#include "modules/register_file.hpp"

// #include "modules/data_memory.hpp"
// #include "modules/gi_gd_register.hpp"
// #include "modules/gd_ex_register.hpp"

int sc_main(int argc, char *argv[]) {

    //          ### CLOCK ###
    sc_clock clock("clock", 100, SC_NS, 0.5);

    //          ### SIGNALS (WIRES) ###
    sc_signal<int> PCjump_address;
    sc_signal<int> PCaddress;

    sc_signal<WORD> IMinst;

    sc_signal<WORD> IFIDinst;

    sc_signal<WORD> ADs;

    // sc_signal<WORD> muxa;
    //sc_signal<WORD> IMinst;

    sc_signal<WORD> IFIDaddress_out;

    sc_signal<WORD> RFdata1, RFdata2;


    //          ### COMPONENTS ###

    program_counter program_counter("program_counter");
    //  -- Input --
    program_counter.clock(clock);
    program_counter.jump_address(PCjump_address);
    //  -- OutPut --
    program_counter.address(PCaddress);

    instruction_memory instruction_memory("instruction_memory");
    instruction_memory.address(PCaddress);
    instruction_memory.inst(IMinst);

    if_id_reg if_id_reg("if_id_reg");
    if_id_reg.clock(clock);
    if_id_reg.inst_in(IMinst);
    if_id_reg.inst_out(IFIDinst);

    register_file register_file("register_file");
    register_file.reg1(IFIDinst);
    register_file.reg2(IFIDinst);
    register_file.write_reg(IFIDinst);
    sc_signal<WORD> blah;
    sc_signal<bool> blah1;
    register_file.write_data(blah);
    register_file.reg_write(blah1);
    register_file.data1(RFdata1);
    register_file.data2(RFdata2);

    //adder adder("adder");
    //adder.a(IMinst);
    //adder.b(IMinst);
    //adder.s(As);


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
    sc_bv<32> inst;

    int i = 0;
    // Checks if there is a instruction opCode in the line
    while (instFs >> opcode_name >> reg1 >> reg2 >> dest) {
        string instruction = "";

        // Convert opcode string to int and save
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

        //std::cout << instruction << std::endl;
        // Assign the binary string to the bit-vector
        for (int j = 0; j < (int) instruction.size(); ++j){
                inst[31-j] = instruction[j];
        }

        //std::cout << inst << std::endl;

        instruction_memory.memory[i] = inst;
        i++;
    }
    instFs.close();

    //          ### TESTBENCH ###

    testbench TestBench("TestBench");
    TestBench.clock(clock);

    //          ### WAVEFORM ###
    sc_trace_file *fp;
    fp = sc_create_vcd_trace_file("wave");
    fp->set_time_unit(1, sc_core::SC_NS);

    sc_trace(fp, clock, "0-0-clock");

    //  ## MEMORY ##
    //sc_trace(fp, DataMemory.memory[0], "DataMemory|0-memory");
    //sc_trace(fp, DataMemory.memory[1], "DataMemory|1-memory");
    //sc_trace(fp, DataMemory.memory[2], "DataMemory|2-memory");
    //sc_trace(fp, DataMemory.memory[3], "DataMemory|3-memory");
    //sc_trace(fp, DataMemory.memory[4], "DataMemory|4-memory");
    //sc_trace(fp, DataMemory.memory[5], "DataMemory|5-memory");
    //sc_trace(fp, DataMemory.memory[6], "DataMemory|6-memory");
    //sc_trace(fp, DataMemory.memory[7], "DataMemory|7-memory");

    ////  ## GIGDRegister $$
    //sc_trace(fp, GIGDRegister.opCodeIn, "GIGDRegister|1-1-opCodeIGI");
    //sc_trace(fp, GIGDRegister.regRead1In, "GIGDRegister|1-2-regRead1IGI");
    //sc_trace(fp, GIGDRegister.regRead2In, "GIGDRegister|1-3-regRead2IGI");
    //sc_trace(fp, GIGDRegister.regWriteIn, "GIGDRegister|1-4-regWriteIGI");

    //sc_trace(fp, GIGDRegister.opCodeOut, "GIGDRegister|2-1-opCodeGIGD");
    //sc_trace(fp, GIGDRegister.regRead1Out, "GIGDRegister|2-2-regRead1GID");
    //sc_trace(fp, GIGDRegister.regRead2Out, "GIGDRegister|2-3-regRead2GID");
    //sc_trace(fp, GIGDRegister.regWriteOut, "GIGDRegister|2-4-regWriteGIGD");

    //// sc_trace(fp, GDEXRegister.opCodeIn,     "2-5-opCodeGIGD");
    //// sc_trace(fp, GDEXRegister.dataRead1In,  "2-6-dataRead1DGD");
    //// sc_trace(fp, GDEXRegister.dataRead2In,  "2-7-dataRead2DGD");
    //// sc_trace(fp, GDEXRegister.dataWriteIn,  "2-8-dataWriteAGD");

    //sc_trace(fp, GDEXRegister.opCodeOut, "GDEXRegister|3-1-opCodeGDA");
    //sc_trace(fp, GDEXRegister.dataRead1Out, "GDEXRegister|3-2-dataRead1GDA");
    //sc_trace(fp, GDEXRegister.dataRead2Out, "GDEXRegister|3-3-dataRead2GDA");
    //sc_trace(fp, GDEXRegister.regWriteOut, "GDEXRegister|3-4-regWriteGDD");
    //sc_trace(fp, GDEXRegister.dataWriteOut, "GDEXRegister|3-4-dataWriteGDD");
    sc_trace(fp, program_counter.jump_address, "program_counter|0-1-jump_address");
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

    //sc_trace(fp, adder.a, "adder|0-1-a");
    //sc_trace(fp, adder.b, "adder|0-2-b");
    //sc_trace(fp, adder.s, "adder|0-3-s");
    sc_start();

    sc_close_vcd_trace_file(fp);

    return 0;
}
