#include "systemc.h"

#include <fstream>

// INCLUDE MODULES
#include "modules/alu.hpp"
#include "modules/mux2x1.hpp"
#include "modules/instruction_memory.hpp"
//#include "modules/register_bank.hpp"
#include "modules/data_memory.hpp"
#include "modules/program_counter.hpp"

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
        sensitive << clock;
    }
};

int sc_main(int argc, char* argv[]) {

    //          ### CLOCK ###
    sc_clock clock("clock", 10, SC_NS, 0.5);

    //          ### SIGNALS (WIRES) ###
    // program_counter to instruction_memory (PI)
    sc_signal<int> addressPI;

    // instruction_memory to data_memory (ID)
    sc_signal<int> regRead1ID, regRead2ID, regWriteID;

    // data_memory to alu (DA)
    sc_signal<int> dataRead1DA, dataRead2DA;

    // instruction_memory to alu (IA)
    sc_signal<int> opCodeIA;

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
    InstructionMemory.opCode(opCodeIA);
    InstructionMemory.regRead1(regRead1ID);
    InstructionMemory.regRead2(regRead2ID);
    InstructionMemory.regWrite(regWriteID);

    data_memory DataMemory("DataMemory");
    //  -- Input --
    DataMemory.regRead1(regRead1ID);
    DataMemory.regRead2(regRead2ID);
    DataMemory.regWrite(regWriteID);
    DataMemory.dataWrite(dataWriteAD);
    //  -- OutPut --
    DataMemory.dataRead1(dataRead1DA);
    DataMemory.dataRead2(dataRead2DA);

    alu Alu ("Alu");
    //  -- Input --
    Alu.opCode(opCodeIA);
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

    // Prints all loaded instructions
    for (int j = 0; j < 10; j++)
        cout << DataMemory.memory[j] << endl;

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

    // Prints all loaded instructions
    for (int j = 0; j < i; j++)
        cout << InstructionMemory.memory[j] << endl;


    testbench TestBench("TestBench");
    TestBench.clock(clock);


    // ### TESTBENCHES ###
    //testbench_ula TbUla("TbUla");
    //TbUla.A(ASig);
    //TbUla.B(BSig);
    //TbUla.CMD(CMDSig);
    //TbUla.Clk(Clock);

    //testbench_mux2x1 TbMux2x1("TbMux");
    //TbMux2x1.a(a);
    //TbMux2x1.b(b);
    //TbMux2x1.sel(sel);
    //TbMux2x1.Clk(Clock);

    // ### MONITORS ###
    //monitor_ula MonUla("MonUla");
    //MonUla.A(ASig);
    //MonUla.B(BSig);
    //MonUla.CMD(CMDSig);
    //MonUla.RES(RESSig);
    //MonUla.Clk(Clock);

    //monitor_mux2x1 MonMux2x1("MonMux2x1");
    //MonMux2x1.a(a);
    //MonMux2x1.b(b);
    //MonMux2x1.sel(sel);
    //MonMux2x1.out(out);
    //MonMux2x1.Clk(Clock);

    //          ### WAVEFORM ###
    sc_trace_file *fp;
    fp=sc_create_vcd_trace_file("wave");
    fp->set_time_unit(1, sc_core::SC_NS);

    sc_trace(fp, clock, "clock");

    sc_trace(fp,InstructionMemory.address,"address");
    //sc_trace(fp,InstructionMemory.opCode,"opCode");
    //sc_trace(fp,InstructionMemory.regRead1,"regRead1");
    //sc_trace(fp,InstructionMemory.regRead2,"regRead2");
    //sc_trace(fp,InstructionMemory.regWrite,"regWrite");

    sc_trace(fp, DataMemory.regRead1, "regRead1");
    sc_trace(fp, DataMemory.regRead2, "regRead2");
    sc_trace(fp, DataMemory.regWrite, "regWrite");
    sc_trace(fp, DataMemory.dataWrite, "dataWrite");

    sc_trace(fp, DataMemory.dataRead1, "dataRead1");
    sc_trace(fp, DataMemory.dataRead2, "dataRead2");

    sc_start();

    sc_close_vcd_trace_file(fp);

    return 0;
}
