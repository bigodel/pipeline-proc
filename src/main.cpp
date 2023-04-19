#include "systemc.h"
#include "testbench.h"
#include "mon.h"

#include "modules/alu.h"

int sc_main(int argc, char* argv[]) {
    
	sc_signal<sc_uint<32>> ASig, BSig, RESSig;
	sc_signal<int> CMDSig;
	sc_clock TestClk("TestClock", 10, SC_NS, 0.5);

	testbench Tb("Stimulus");
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

	sc_start();

	sc_close_vcd_trace_file(fp);

	return 0;
}
