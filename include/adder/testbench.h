#include "systemc.h"

SC_MODULE(testbench) {
	sc_out<bool> A, B;
	sc_in<bool> Clk;

	void TbGen() {
		A.write(false);
		B.write(false); 
		
		wait();

		A.write(false);
		B.write(true);

		wait();
		
		A.write(true);
		B.write(false);
		
		wait();
		
		A.write(true);
		B.write(true);
		
		wait();
		
		sc_stop();
	}

	SC_CTOR(testbench) {
		SC_THREAD(TbGen);
		sensitive << Clk.pos();
	}
};
