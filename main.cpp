#include <systemc.h>
#include "mux.h"
#include "testbench.h"
// #include "include/adder/mon.h"
// #include "include/adder/adder.h"

int sc_main(int argc, char* argv[]) {
    sc_signal<WORD_TYPE> a, b, sel;

    testbench tb("mux testbench");
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

    return 0;
}

// int sc_main(int argc, char* argv[]) {
//     
// 	sc_signal<bool> ASig, BSig, SSig, COSig;
// 	sc_clock TestClk("TestClock", 10, SC_NS, 0.5);
// 
// 	testbench Tb("Stimulus");
// 	Tb.A(ASig);
// 	Tb.B(BSig);
// 	Tb.Clk(TestClk);
// 
// 	adder Somador("adder");
// 	Somador.A(ASig);
// 	Somador.B(BSig);
// 	Somador.S(SSig);
// 	Somador.CO(COSig);
// 
// 	mon Monitor1("Monitor");
// 	Monitor1.A(ASig);
// 	Monitor1.B(BSig);
// 	Monitor1.S(SSig);
// 	Monitor1.CO(COSig);
// 	Monitor1.Clk(TestClk);
// 
// 	//========================= waveform
// 	sc_trace_file *fp;
// 	fp=sc_create_vcd_trace_file("wave");
// 	fp->set_time_unit(1, sc_core::SC_NS);
// 	sc_trace(fp,Somador.A,"A");
// 	sc_trace(fp,Somador.B,"B");
// 	sc_trace(fp,Somador.CO,"CO");
// 	sc_trace(fp,TestClk,"CLK");
// 	//=========================
// 
// 	sc_start();
// 
// 	sc_close_vcd_trace_file(fp);
// 
// 	return 0;
// }
