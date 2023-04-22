#include <systemc.h>
#include "testbench.hpp"
#include "mon.hpp"

#include "modules/alu.hpp"
#include "modules/mux.hpp"

void stimulus_test() {
    sc_signal<sc_uint<32>> ASig, BSig, RESSig;
    sc_signal<int> CMDSig;
    sc_clock TestClk("TestClock", 10, SC_NS, 0.5);

    cmd_testbench Tb("Stimulus");
    Tb.A(ASig);
    Tb.B(BSig);
    Tb.CMD(CMDSig);
    Tb.Clk(TestClk);

    alu ALU ("alu");
    ALU.A(ASig);
    ALU.B(BSig);
    ALU.result(RESSig);
    ALU.command(CMDSig);

    mon Monitor1("Monitor");
    Monitor1.A(ASig);
    Monitor1.B(BSig);
    Monitor1.CMD(CMDSig);
    Monitor1.RES(RESSig);
    Monitor1.Clk(TestClk);

    //========================= waveform
    sc_trace_file *fp;
    fp=sc_create_vcd_trace_file("wave");
    fp->set_time_unit(1, sc_core::SC_NS);
    sc_trace(fp,ALU.A,"A");
    sc_trace(fp,ALU.B,"B");
    sc_trace(fp,TestClk,"CLK");
    //=========================

    sc_close_vcd_trace_file(fp);
}

void mux_test() {
    sc_signal<sc_uint<32>> a, b;
    sc_signal<bool> sel;

    mux_testbench tb("mux testbench");
    tb.a(a);
    tb.b(b);
    tb.sel(sel);

    mux2x1 mux("mux 2x1");
    mux.a(a);
    mux.b(b);
    mux.sel(sel);

    //========================= waveform
    sc_trace_file *fp;
    fp = sc_create_vcd_trace_file("wave");
    fp->set_time_unit(1, sc_core::SC_NS);
    sc_trace(fp, mux.a,"A");
    sc_trace(fp, mux.b,"B");
    sc_trace(fp, mux.sel,"SEL");
    //=========================

    sc_start();

    sc_close_vcd_trace_file(fp);
}

int sc_main(int argc, char* argv[]) {
    stimulus_test();

    return 0;
}

// int sc_main(int argc, char* argv[]) {
//     
//      sc_signal<bool> ASig, BSig, SSig, COSig;
//      sc_clock TestClk("TestClock", 10, SC_NS, 0.5);
// 
//      testbench Tb("Stimulus");
//      Tb.A(ASig);
//      Tb.B(BSig);
//      Tb.Clk(TestClk);
// 
//      adder Somador("adder");
//      Somador.A(ASig);
//      Somador.B(BSig);
//      Somador.S(SSig);
//      Somador.CO(COSig);
// 
//      mon Monitor1("Monitor");
//      Monitor1.A(ASig);
//      Monitor1.B(BSig);
//      Monitor1.S(SSig);
//      Monitor1.CO(COSig);
//      Monitor1.Clk(TestClk);
// 
//      //========================= waveform
//      sc_trace_file *fp;
//      fp=sc_create_vcd_trace_file("wave");
//      fp->set_time_unit(1, sc_core::SC_NS);
//      sc_trace(fp,Somador.A,"A");
//      sc_trace(fp,Somador.B,"B");
//      sc_trace(fp,Somador.CO,"CO");
//      sc_trace(fp,TestClk,"CLK");
//      //=========================
// 
//      sc_start();
// 
//      sc_close_vcd_trace_file(fp);
// 
//      return 0;
// }
