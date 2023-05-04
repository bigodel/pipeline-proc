#include "systemc.h"

#include <fstream>

// INCLUDE MODULES
//#include "modules/alu.hpp"
#include "modules/mux2x1.hpp"
#include "modules/instruction_memory.hpp"
#include "modules/data_memory.hpp"

// INCLUDE TESTBENCHES
#include "testbenches/testbench_alu.hpp"
#include "testbenches/testbench_mux2x1.hpp"

// INCLUDE MONITORS
#include "monitors/monitor_alu.hpp"
#include "monitors/monitor_mux2x1.hpp"


int sc_main(int argc, char* argv[]) {

    // ### SIGNALS (WIRES) ###

    //// ula
    sc_signal<int> regStartSig, regTermSig, opCodeSig, regDestSig;

    // mux2x1
    sc_signal<sc_uint<32>> a, b, out;
    sc_signal<bool> sel;

    // instuction_memory
    sc_signal<int> addressIM;
    sc_signal<inst> instructionIM;

    // data_memory
    sc_signal<int> addressDM, input_dataDM, dataDM;
    sc_signal<bool> enableDM;

    // ### CLOCK ###
    sc_clock Clock("Clock", 10, SC_NS, 0.5);

    // ### COMPONENTS ###

    //alu Alu ("Alu");
    //Alu.regStart(regStartSig);
    //Alu.regTerm(regTermSig);
    //Alu.opCode(opCodeSig);
    //Alu.regDest(regDestSig);

    mux2x1 Mux2x1("Mux2x1");
    Mux2x1.a(a);
    Mux2x1.b(b);
    Mux2x1.sel(sel);
    Mux2x1.out(out);

    instruction_memory InstructionMemory("InstructionMemory");
    InstructionMemory.address(addressIM);
    InstructionMemory.instruction(instructionIM);

    data_memory DataMemory("DataMemory");
    DataMemory.address(addressDM);
    DataMemory.input_data(input_dataDM);
    DataMemory.enable(enableDM);
    DataMemory.data(dataDM);

    // # READ DATA FILE AND LOAD INTO DATA MEMORY #
    fstream instFs;

    instFs.open("data.in");

    // Checks if the file was found
    if(!instFs) {
        cerr << "Error: file could not be opened" << endl;
        return 1;
    }

    // Loads all instructions into the memory
    int address = 0;
    while (instFs >> address) {
        instFs >> DataMemory.mem[address];
    }
    instFs.close();

    // Prints all loaded instructions
    for (int j = 0; j < 10; j++)
        cout << DataMemory.mem[j] << endl;

    // # READ INSTRUCTION FILE AND LOAD INTO INSTRUCTION MEMORY #
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
        if(opCodeName == "AND")
            InstructionMemory.mem[i] = 0;
        else if(opCodeName == "OR")
            InstructionMemory.mem[i] = 1;
        else if(opCodeName == "XOR")
            InstructionMemory.mem[i] = 2;
        else if(opCodeName == "NOT")
            InstructionMemory.mem[i] = 3;
        else if(opCodeName == "CMP")
            InstructionMemory.mem[i] = 4;
        else if(opCodeName == "ADD")
            InstructionMemory.mem[i] = 5;
        else if(opCodeName == "SUB")
            InstructionMemory.mem[i] = 6;

        // If so, get all the registers address
        instFs >> InstructionMemory.mem[i].regStart;
        instFs >> InstructionMemory.mem[i].regTerm;
        instFs >> InstructionMemory.mem[i].regDest;
        i++;
    }
    instFs.close();

    // Prints all loaded instructions
    for (int j = 0; j < i; j++)
        cout << InstructionMemory.mem[j] << endl;

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

    // WAVEFORM
    //sc_trace_file *fp;
    //fp=sc_create_vcd_trace_file("wave");
    //fp->set_time_unit(1, sc_core::SC_NS);
    //sc_trace(fp,ALU.A,"A");
    //sc_trace(fp,ALU.B,"B");
    //sc_trace(fp,Clock,"CLK");
    //=========================

    sc_start();

    //sc_close_vcd_trace_file(fp);

    return 0;
}
