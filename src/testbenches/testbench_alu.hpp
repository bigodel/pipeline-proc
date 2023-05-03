#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(testbench_alu) {
    sc_out<WORD> A, B;
    sc_out<int> CMD;
    sc_in<bool> Clk;

    void TbGen() {
        // ADD
        A.write(1);
        B.write(0);
        CMD.write(0);

        wait();

        // OR
        A.write(1);
        B.write(0);
        CMD.write(1);

        wait();

        // ADD
        A.write(3);
        B.write(4);
        CMD.write(2);

        wait();

        // SUB
        A.write(4);
        B.write(3);
        CMD.write(3);

        wait();

        // Set-on-less-than
        A.write(4);
        B.write(3);
        CMD.write(4);

        wait();

        sc_stop();
    }

    SC_CTOR(testbench_alu) {
        SC_THREAD(TbGen);
        sensitive << Clk.pos();
    }
};
