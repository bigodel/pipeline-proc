#include "systemc.h"

#include <fstream>

// INCLUDE MODULES
// #include "modules/alu.hpp"
// #include "modules/register_file.hpp"
// #include "modules/mux2x1.hpp"
// #include "modules/instruction_memory.hpp"
// #include "modules/data_memory.hpp"
// #include "modules/program_counter.hpp"
// #include "modules/gi_gd_register.hpp"
// #include "modules/gd_ex_register.hpp"

int sc_main(int argc, char* argv[]) {
    sc_clock clock("clock", 100, SC_NS, 0.5);

    sc_start();

    return 0;
}

// SC_MODULE(testbench) {
//     sc_in_clk clock;

//     void test() {
//         for (size_t i = 0; i < 10; i++) {
//             wait();
//         }

//         sc_stop();
//     }

//     SC_CTOR(testbench) {
//         SC_THREAD(test);
//         sensitive << clock.pos();
//     }
// };

// int sc_main(int argc, char* argv[]) {

//     //          ### CLOCK ###
//     sc_clock clock("clock", 100, SC_NS, 0.5);

//     //          ### SIGNALS (WIRES) ###
//     // program_counter to instruction_memory (PI)
//     sc_signal<int> addressPI;

//     // instruction_memory to gi_gd_register (IGI)
//     sc_signal<int> regRead1IGI, regRead2IGI, regWriteIGI, opCodeIGI;

//     // gi_gd_register ot data_memory (GID)
//     sc_signal<int> regRead1GID, regRead2GID;

//     // gi_gd_register to gd_ex_register  (GIGD)
//     sc_signal<int> opCodeGIGD, regWriteGIGD;

//     // gd_ex_register to alu (GDA)
//     sc_signal<int> opCodeGDA;

//     // data_memory to gd_ex_register (DGD)
//     sc_signal<int> dataRead1DGD, dataRead2DGD;

//     // gd_ex_register to alu (GDA)
//     sc_signal<WORD> dataRead1GDA, dataRead2GDA;

//     // alu to gd_ex_register (AGD)
//     sc_signal<WORD> dataWriteAGD;

//     // gd_ex_register to data_memory (GDD)
//     sc_signal<int> dataWriteGDD, regWriteGDD;

//     //          ### COMPONENTS ###

//     program_counter ProgramCounter("ProgramCounter");
//     //  -- Input --
//     ProgramCounter.clock(clock);
//     //  -- OutPut --
//     ProgramCounter.address(addressPI);

//     instruction_memory InstructionMemory("InstructionMemory");
//     //  -- Input --
//     InstructionMemory.address(addressPI);
//     //  -- OutPut --
//     InstructionMemory.opCode(opCodeIGI);
//     InstructionMemory.regRead1(regRead1IGI);
//     InstructionMemory.regRead2(regRead2IGI);
//     InstructionMemory.regWrite(regWriteIGI);

//     gi_gd_register GIGDRegister("GIGDRegister");
//     //  -- Input --
//     GIGDRegister.clock(clock);
//     GIGDRegister.opCodeIn(opCodeIGI);
//     GIGDRegister.regRead1In(regRead1IGI);
//     GIGDRegister.regRead2In(regRead2IGI);
//     GIGDRegister.regWriteIn(regWriteIGI);
//     //  -- OutPut --
//     GIGDRegister.opCodeOut(opCodeGIGD);
//     GIGDRegister.regRead1Out(regRead1GID);
//     GIGDRegister.regRead2Out(regRead2GID);
//     GIGDRegister.regWriteOut(regWriteGIGD);

//     data_memory DataMemory("DataMemory");
//     //  -- Input --
//     DataMemory.regRead1(regRead1GID);
//     DataMemory.regRead2(regRead2GID);
//     DataMemory.regWrite(regWriteGDD);
//     DataMemory.dataWrite(dataWriteGDD);
//     //  -- OutPut --
//     DataMemory.dataRead1(dataRead1DGD);
//     DataMemory.dataRead2(dataRead2DGD);

//     gd_ex_register GDEXRegister("GDEXRegister");
//     //  -- Input --
//     GDEXRegister.clock(clock);
//     GDEXRegister.opCodeIn(opCodeGIGD);
//     GDEXRegister.dataRead1In(dataRead1DGD);
//     GDEXRegister.dataRead2In(dataRead2DGD);
//     GDEXRegister.regWriteIn(regWriteGIGD);
//     GDEXRegister.dataWriteIn(dataWriteAGD);
//     // -- OutPut --
//     GDEXRegister.opCodeOut(opCodeGDA);
//     GDEXRegister.dataRead1Out(dataRead1GDA);
//     GDEXRegister.dataRead2Out(dataRead2GDA);
//     GDEXRegister.regWriteOut(regWriteGDD);
//     GDEXRegister.dataWriteOut(dataWriteGDD);

//     alu Alu ("Alu");
//     //  -- Input --
//     Alu.alu_op(opCodeGDA);
//     Alu.a(dataRead1GDA);
//     Alu.b(dataRead2GDA);
//     //  -- OutPut --
//     Alu.result(dataWriteAGD);

//     // # READ DATA FILE AND LOAD INTO DATA MEMORY #
//     fstream instFs;

//     instFs.open("data.in");

//     // Checks if the file was found
//     if(!instFs) {
//         cerr << "Error: file could not be opened" << endl;
//         return 1;
//     }

//     // Loads all instructions into the memory
//     int dest = 0;
//     while (instFs >> dest) {
//         instFs >> DataMemory.memory[dest];
//     }
//     instFs.close();

//     //  # READ INSTRUCTION FILE AND LOAD INTO INSTRUCTION MEMORY #
//     instFs.open("instruction1.in");

//     // Checks if the file was found
//     if(!instFs) {
//         cerr << "Error: file could not be opened" << endl;
//         return 1;
//     }

//     // Loads all instructions into the memory
//     int i = 0;
//     string opCodeName = "";
//     // Checks if there is a instruction opCode in the line
//     while (instFs >> opCodeName) {
//         // Convert opCode string to int and save
//         if(opCodeName == "AND")
//             InstructionMemory.memory[i].opCode = 0;
//         else if(opCodeName == "OR")
//             InstructionMemory.memory[i].opCode = 1;
//         else if(opCodeName == "XOR")
//             InstructionMemory.memory[i].opCode = 2;
//         else if(opCodeName == "NOT")
//             InstructionMemory.memory[i].opCode = 3;
//         else if(opCodeName == "CMP")
//             InstructionMemory.memory[i].opCode = 4;
//         else if(opCodeName == "ADD")
//             InstructionMemory.memory[i].opCode = 5;
//         else if(opCodeName == "SUB")
//             InstructionMemory.memory[i].opCode = 6;

//         // Get all the regIndexisters address and save
//         instFs >> InstructionMemory.memory[i].regRead1;
//         instFs >> InstructionMemory.memory[i].regRead2;
//         instFs >> InstructionMemory.memory[i].regWrite;
//         i++;
//     }
//     instFs.close();

//     //          ### TESTBENCH ###

//     testbench TestBench("TestBench");
//     TestBench.clock(clock);

//     //          ### WAVEFORM ###
//     sc_trace_file *fp;
//     fp=sc_create_vcd_trace_file("wave");
//     fp->set_time_unit(1, sc_core::SC_NS);

//     sc_trace(fp, clock,                        "0-0-clock");

//     //  ## MEMORY ##
//     sc_trace(fp, DataMemory.memory[0],         "DataMemory|0-memory");
//     sc_trace(fp, DataMemory.memory[1],         "DataMemory|1-memory");
//     sc_trace(fp, DataMemory.memory[2],         "DataMemory|2-memory");
//     sc_trace(fp, DataMemory.memory[3],         "DataMemory|3-memory");
//     sc_trace(fp, DataMemory.memory[4],         "DataMemory|4-memory");
//     sc_trace(fp, DataMemory.memory[5],         "DataMemory|5-memory");
//     sc_trace(fp, DataMemory.memory[6],         "DataMemory|6-memory");
//     sc_trace(fp, DataMemory.memory[7],         "DataMemory|7-memory");

//     //  ## GIGDRegister $$
//     sc_trace(fp, GIGDRegister.opCodeIn,     "GIGDRegister|1-1-opCodeIGI");
//     sc_trace(fp, GIGDRegister.regRead1In,   "GIGDRegister|1-2-regRead1IGI");
//     sc_trace(fp, GIGDRegister.regRead2In,   "GIGDRegister|1-3-regRead2IGI");
//     sc_trace(fp, GIGDRegister.regWriteIn,   "GIGDRegister|1-4-regWriteIGI");

//     sc_trace(fp, GIGDRegister.opCodeOut,    "GIGDRegister|2-1-opCodeGIGD");
//     sc_trace(fp, GIGDRegister.regRead1Out,  "GIGDRegister|2-2-regRead1GID");
//     sc_trace(fp, GIGDRegister.regRead2Out,  "GIGDRegister|2-3-regRead2GID");
//     sc_trace(fp, GIGDRegister.regWriteOut,  "GIGDRegister|2-4-regWriteGIGD");

//     //sc_trace(fp, GDEXRegister.opCodeIn,     "2-5-opCodeGIGD");
//     //sc_trace(fp, GDEXRegister.dataRead1In,  "2-6-dataRead1DGD");
//     //sc_trace(fp, GDEXRegister.dataRead2In,  "2-7-dataRead2DGD");
//     //sc_trace(fp, GDEXRegister.dataWriteIn,  "2-8-dataWriteAGD");

//     sc_trace(fp, GDEXRegister.opCodeOut,    "GDEXRegister|3-1-opCodeGDA");
//     sc_trace(fp, GDEXRegister.dataRead1Out, "GDEXRegister|3-2-dataRead1GDA");
//     sc_trace(fp, GDEXRegister.dataRead2Out, "GDEXRegister|3-3-dataRead2GDA");
//     sc_trace(fp, GDEXRegister.regWriteOut,  "GDEXRegister|3-4-regWriteGDD");
//     sc_trace(fp, GDEXRegister.dataWriteOut, "GDEXRegister|3-4-dataWriteGDD");

//     sc_start();

//     sc_close_vcd_trace_file(fp);

//     return 0;
// }
