#include "systemc.h"

#include <fstream>

// INCLUDE MODULES
#include "modules/alu.hpp"
#include "modules/mux2x1.hpp"
#include "modules/instruction_memory.hpp"
#include "modules/data_memory.hpp"
#include "modules/program_counter.hpp"
#include "modules/gi_gd_register.hpp"

// INCLUDE TESTBENCHES
#include "testbenches/testbench_alu.hpp"
#include "testbenches/testbench_mux2x1.hpp"

// INCLUDE MONITORS
#include "monitors/monitor_alu.hpp"
#include "monitors/monitor_mux2x1.hpp"

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

int sc_main(int argc, char* argv[]) {

    //          ### CLOCK ###
    sc_clock clock("clock", 100, SC_NS, 0.5);

    //          ### SIGNALS (WIRES) ###
    // program_counter to instruction_memory (PI)
    sc_signal<int> addressPI;

    // instruction_memory to gi_gd_register (IGI)
    sc_signal<int> regRead1IGI, regRead2IGI, regWriteIGI, opCodeIGI;

    // gi_gd_register ot data_memory (GID)
    sc_signal<int> regRead1GID, regRead2GID, regWriteGID;

    // data_memory to alu (DA)
    sc_signal<int> dataRead1DA, dataRead2DA;

    // gi_gd_register to alu (GIA)
    sc_signal<int> opCodeGIA;

    // alu to data_memory (AD)
    sc_signal<int> dataWriteAD;

    // control_part (TODO) to data_memory (CD)
    //sc_signal<bool> isReadingCD;

    //          ### COMPONENTS ###

    program_counter ProgramCounter("ProgramCounter");
    //  -- Input --
    ProgramCounter.clock(clock);
    //  -- OutPut --
    ProgramCounter.address(addressPI);

    instruction_memory InstructionMemory("InstructionMemory");
    //  -- Input --
    InstructionMemory.address(addressPI);
    //  -- OutPut --
    InstructionMemory.opCode(opCodeIGI);
    InstructionMemory.regRead1(regRead1IGI);
    InstructionMemory.regRead2(regRead2IGI);
    InstructionMemory.regWrite(regWriteIGI);

    gi_gd_register GIGDRegister("GIGDRegister");
    //  -- Input --
    GIGDRegister.clock(clock);
    GIGDRegister.opCodeIn(opCodeIGI);
    GIGDRegister.regRead1In(regRead1IGI);
    GIGDRegister.regRead2In(regRead2IGI);
    GIGDRegister.regWriteIn(regWriteIGI);
    //  -- OutPut --
    GIGDRegister.opCodeOut(opCodeGIA);
    GIGDRegister.regRead1Out(regRead1GID);
    GIGDRegister.regRead2Out(regRead2GID);
    GIGDRegister.regWriteOut(regWriteGID);

    data_memory DataMemory("DataMemory");
    //  -- Input --
    DataMemory.regRead1(regRead1GID);
    DataMemory.regRead2(regRead2GID);
    DataMemory.regWrite(regWriteGID);
    DataMemory.dataWrite(dataWriteAD);
    //  -- OutPut --
    DataMemory.dataRead1(dataRead1DA);
    DataMemory.dataRead2(dataRead2DA);

    alu Alu ("Alu");
    //  -- Input --
    Alu.opCode(opCodeGIA);
    Alu.dataRead1(dataRead1DA);
    Alu.dataRead2(dataRead2DA);
    //  -- OutPut --
    Alu.dataWrite(dataWriteAD);


    // # READ DATA FILE AND LOAD INTO DATA MEMORY #
    fstream instFs;

    instFs.open("data.in");

    // Checks if the file was found
    if(!instFs) {
        cerr << "Error: file could not be opened" << endl;
        return 1;
    }

    // Loads all instructions into the memory
    int dest = 0;
    while (instFs >> dest) {
        instFs >> DataMemory.memory[dest];
    }
    instFs.close();

    //  # READ INSTRUCTION FILE AND LOAD INTO INSTRUCTION MEMORY #
    instFs.open("instruction.in");

    // Checks if the file was found
    if(!instFs) {
        cerr << "Error: file could not be opened" << endl;
        return 1;
    }

    // Loads all instructions into the memory
    int i = 0;
    string opCodeName = "";
    // Checks if there is a instruction opCode in the line
    while (instFs >> opCodeName) {
        // Convert opCode string to int and save
        if(opCodeName == "AND")
            InstructionMemory.memory[i].opCode = 0;
        else if(opCodeName == "OR")
            InstructionMemory.memory[i].opCode = 1;
        else if(opCodeName == "XOR")
            InstructionMemory.memory[i].opCode = 2;
        else if(opCodeName == "NOT")
            InstructionMemory.memory[i].opCode = 3;
        else if(opCodeName == "CMP")
            InstructionMemory.memory[i].opCode = 4;
        else if(opCodeName == "ADD")
            InstructionMemory.memory[i].opCode = 5;
        else if(opCodeName == "SUB")
            InstructionMemory.memory[i].opCode = 6;

        // Get all the regIndexisters address and save
        instFs >> InstructionMemory.memory[i].regRead1;
        instFs >> InstructionMemory.memory[i].regRead2;
        instFs >> InstructionMemory.memory[i].regWrite;
        i++;
    }
    instFs.close();

    testbench TestBench("TestBench");
    TestBench.clock(clock);

    //          ### WAVEFORM ###
    sc_trace_file *fp;
    fp=sc_create_vcd_trace_file("wave");
    fp->set_time_unit(1, sc_core::SC_NS);

    sc_trace(fp, clock, "clock");

    //sc_trace(fp,InstructionMemory.address,"address");
    //sc_trace(fp,InstructionMemory.opCode,"opCode");
    //sc_trace(fp,InstructionMemory.regRead1,"regRead1");
    //sc_trace(fp,InstructionMemory.regRead2,"regRead2");
    //sc_trace(fp,InstructionMemory.regWrite,"regWrite");

    //sc_trace(fp, DataMemory.regRead1, "regRead1");
    //sc_trace(fp, DataMemory.regRead2, "regRead2");
    //sc_trace(fp, DataMemory.regWrite, "regWrite");
    sc_trace(fp, DataMemory.dataWrite, "dataWrite");

    //sc_trace(fp, DataMemory.dataRead1, "dataRead1");
    //sc_trace(fp, DataMemory.dataRead2, "dataRead2");

    //sc_trace(fp, GIGDRegister.clock, "clock");
    sc_trace(fp, GIGDRegister.opCodeIn, "opCodeIGI");
    sc_trace(fp, GIGDRegister.regRead1In, "regRead1IGI");
    sc_trace(fp, GIGDRegister.regRead2In, "regRead2IGI");
    sc_trace(fp, GIGDRegister.regWriteIn, "regWriteIGI");

    sc_trace(fp, GIGDRegister.opCodeOut, "opCodeGIA");
    sc_trace(fp, GIGDRegister.regRead1Out, "regRead1GID");
    sc_trace(fp, GIGDRegister.regRead2Out, "regRead2GID");
    sc_trace(fp, GIGDRegister.regWriteOut, "regWriteGID");


    sc_start();

    sc_close_vcd_trace_file(fp);

    return 0;
}
