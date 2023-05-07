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
#include "modules/program_counter.hpp"
#include "modules/register_file.hpp"

// #include "modules/data_memory.hpp"
// #include "modules/gi_gd_register.hpp"
// #include "modules/gd_ex_register.hpp"

int sc_main(int argc, char *argv[]) {

    //          ### CLOCK ###
    sc_clock clock("clock", 100, SC_NS, 0.5);

    //          ### SIGNALS (WIRES) ###
    // program_counter to instruction_memory (PI)
    // sc_signal<int> addressPI;

    //// instruction_memory to gi_gd_register (IGI)
    // sc_signal<int> regRead1IGI, regRead2IGI, regWriteIGI, opCodeIGI;

    //// gi_gd_register ot data_memory (GID)
    // sc_signal<int> regRead1GID, regRead2GID;

    //// gi_gd_register to gd_ex_register  (GIGD)
    // sc_signal<int> opCodeGIGD, regWriteGIGD;

    //// gd_ex_register to alu (GDA)
    // sc_signal<int> opCodeGDA;

    //// data_memory to gd_ex_register (DGD)
    // sc_signal<int> dataRead1DGD, dataRead2DGD;

    //// gd_ex_register to alu (GDA)
    // sc_signal<WORD> dataRead1GDA, dataRead2GDA;

    //// alu to gd_ex_register (AGD)
    // sc_signal<WORD> dataWriteAGD;

    //// gd_ex_register to data_memory (GDD)
    // sc_signal<int> dataWriteGDD, regWriteGDD;

    sc_signal<int> PCjump_address;
    sc_signal<int> PCaddress;

    sc_signal<WORD> IMinst;

    sc_signal<WORD> ADs;

    // sc_signal<WORD> muxa;
    sc_signal<WORD> IMinst;

    sc_signal<WORD> IFIDaddress_out;

    sc_signal<WORD> RFreg1;
    sc_signal<WORD> RFreg2;

    //          ### COMPONENTS ###

    program_counter ProgramCounter("ProgramCounter");
    //  -- Input --
    ProgramCounter.clock(clock);
    ProgramCounter.jump_address(PCjump_address);
    //  -- OutPut --
    ProgramCounter.address(PCaddress);

    instruction_memory InstructionMemory("InstructionMemory");
    InstructionMemory.address(PCadress);
    InstructionMemory.inst(IMinst);

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
    instFs.open("instruction1.in");

    // Checks if the file was found
    if (!instFs) {
        cerr << "Error: file could not be opened" << endl;
        return 1;
    }

    // Loads all instructions into the memory
    string opcode_name = "";
    int reg1, reg2, dest;

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
    sc_trace(fp, DataMemory.memory[0], "DataMemory|0-memory");
    sc_trace(fp, DataMemory.memory[1], "DataMemory|1-memory");
    sc_trace(fp, DataMemory.memory[2], "DataMemory|2-memory");
    sc_trace(fp, DataMemory.memory[3], "DataMemory|3-memory");
    sc_trace(fp, DataMemory.memory[4], "DataMemory|4-memory");
    sc_trace(fp, DataMemory.memory[5], "DataMemory|5-memory");
    sc_trace(fp, DataMemory.memory[6], "DataMemory|6-memory");
    sc_trace(fp, DataMemory.memory[7], "DataMemory|7-memory");

    //  ## GIGDRegister $$
    sc_trace(fp, GIGDRegister.opCodeIn, "GIGDRegister|1-1-opCodeIGI");
    sc_trace(fp, GIGDRegister.regRead1In, "GIGDRegister|1-2-regRead1IGI");
    sc_trace(fp, GIGDRegister.regRead2In, "GIGDRegister|1-3-regRead2IGI");
    sc_trace(fp, GIGDRegister.regWriteIn, "GIGDRegister|1-4-regWriteIGI");

    sc_trace(fp, GIGDRegister.opCodeOut, "GIGDRegister|2-1-opCodeGIGD");
    sc_trace(fp, GIGDRegister.regRead1Out, "GIGDRegister|2-2-regRead1GID");
    sc_trace(fp, GIGDRegister.regRead2Out, "GIGDRegister|2-3-regRead2GID");
    sc_trace(fp, GIGDRegister.regWriteOut, "GIGDRegister|2-4-regWriteGIGD");

    // sc_trace(fp, GDEXRegister.opCodeIn,     "2-5-opCodeGIGD");
    // sc_trace(fp, GDEXRegister.dataRead1In,  "2-6-dataRead1DGD");
    // sc_trace(fp, GDEXRegister.dataRead2In,  "2-7-dataRead2DGD");
    // sc_trace(fp, GDEXRegister.dataWriteIn,  "2-8-dataWriteAGD");

    sc_trace(fp, GDEXRegister.opCodeOut, "GDEXRegister|3-1-opCodeGDA");
    sc_trace(fp, GDEXRegister.dataRead1Out, "GDEXRegister|3-2-dataRead1GDA");
    sc_trace(fp, GDEXRegister.dataRead2Out, "GDEXRegister|3-3-dataRead2GDA");
    sc_trace(fp, GDEXRegister.regWriteOut, "GDEXRegister|3-4-regWriteGDD");
    sc_trace(fp, GDEXRegister.dataWriteOut, "GDEXRegister|3-4-dataWriteGDD");

    sc_start();

    sc_close_vcd_trace_file(fp);

    return 0;
}
