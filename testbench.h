#include <systemc.h>
#include "defines.h"

SC_MODULE(testbench) {
	sc_out<WORD_TYPE> a, b, sel;

	void test() {
		a.write(10);
		b.write(20); 
        sel.write(true);
		
		wait();

        sel.write(false);

		sc_stop();
	}

	SC_CTOR(testbench) {
		SC_THREAD(test);
	}
};
