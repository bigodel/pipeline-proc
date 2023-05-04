#include "systemc.h"

// INCLUDE MODULES
#include "modules/alu.hpp"
#include "modules/mux2x1.hpp"
#include "modules/instruction_memory.hpp"

// INCLUDE TESTBENCHES
#include "testbenches/testbench_alu.hpp"
#include "testbenches/testbench_mux2x1.hpp"

// INCLUDE MONITORS
#include "monitors/monitor_alu.hpp"
#include "monitors/monitor_mux2x1.hpp"


int sc_main(int argc, char* argv[]) {

    // Para a ula
    sc_signal<sc_uint<32>> ASig, BSig, RESSig;
    sc_signal<int> CMDSig;

    // Para o mux2x1
    sc_signal<sc_uint<32>> a, b, out;
    sc_signal<bool> sel;

    sc_clock TestClk("TestClock", 10, SC_NS, 0.5);

    // COMPONENTS
    alu Alu ("Alu");
    Alu.A(ASig);
    Alu.B(BSig);
    Alu.result(RESSig);
    Alu.command(CMDSig);

    mux2x1 Mux2x1("Mux2x1");
    Mux2x1.a(a);
    Mux2x1.b(b);
    Mux2x1.sel(sel);
    Mux2x1.out(out);

    // TESTBENCHES
    //testbench_ula TbUla("TbUla");
    //TbUla.A(ASig);
    //TbUla.B(BSig);
    //TbUla.CMD(CMDSig);
    //TbUla.Clk(TestClk);

    testbench_mux2x1 TbMux2x1("TbMux");
    TbMux2x1.a(a);
    TbMux2x1.b(b);
    TbMux2x1.sel(sel);
    TbMux2x1.Clk(TestClk);

    // MONITORS
    //monitor_ula MonUla("MonUla");
    //MonUla.A(ASig);
    //MonUla.B(BSig);
    //MonUla.CMD(CMDSig);
    //MonUla.RES(RESSig);
    //MonUla.Clk(TestClk);

    monitor_mux2x1 MonMux2x1("MonMux2x1");
    MonMux2x1.a(a);
    MonMux2x1.b(b);
    MonMux2x1.sel(sel);
    MonMux2x1.out(out);
    MonMux2x1.Clk(TestClk);

    // WAVEFORM
    //sc_trace_file *fp;
    //fp=sc_create_vcd_trace_file("wave");
    //fp->set_time_unit(1, sc_core::SC_NS);
    //sc_trace(fp,ALU.A,"A");
    //sc_trace(fp,ALU.B,"B");
    //sc_trace(fp,TestClk,"CLK");
    //=========================

    sc_start();

    //sc_close_vcd_trace_file(fp);

    return 0;
}
