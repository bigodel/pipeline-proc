#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(testbench_mux2x1) {
    sc_out<WORD> a, b;
    sc_out<bool> sel;
    sc_in<bool> Clk;

    void test() {
        a.write(10);
        b.write(20);

        wait();

        sel.write(true);

        wait();

        sel.write(false);

        wait();

        sc_stop();
    }

    SC_CTOR(testbench_mux2x1) {
        SC_THREAD(test);
        sensitive << Clk.pos();
    }
};
