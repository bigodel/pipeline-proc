#include <iostream>
#include "systemc.h"
using namespace std;

SC_MODULE(gi_gd_register) {
    sc_in_clk clock;
    sc_in<int> opCodeIn, regRead1In, regRead2In, regWriteIn;
    sc_out<int> opCodeOut, regRead1Out, regRead2Out, regWriteOut;

    int memory[4] = {0};

    void read_write() {
        // Read
        memory[0] = opCodeIn.read();
        memory[1] = regRead1In.read();
        memory[2] = regRead2In.read();
        memory[3] = regWriteIn.read();

        // Write
        opCodeOut.write(memory[0]);
        regRead1Out.write(memory[1]);
        regRead2Out.write(memory[2]);
        regWriteOut.write(memory[3]);
    }

    SC_CTOR(gi_gd_register) {
        SC_METHOD(read_write);
        sensitive << clock;
    }
};
