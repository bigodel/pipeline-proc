#include "systemc.h"

SC_MODULE(gd_ex_register) {
    sc_in_clk clock;
    sc_in<int> opCodeIn, dataRead1In, dataRead2In, dataWriteIn, regWriteIn;
    sc_out<int> opCodeOut, dataRead1Out, dataRead2Out, dataWriteOut, regWriteOut;

    int memory[4] = {0};

    void read_write() {
        // Read
        memory[0] = opCodeIn.read();
        memory[1] = dataRead1In.read();
        memory[2] = dataRead2In.read();
        memory[3] = dataWriteIn.read();
        memory[4] = regWriteIn.read();

        // Write
        opCodeOut.write(memory[0]);
        dataRead1Out.write(memory[1]);
        dataRead2Out.write(memory[2]);
        dataWriteOut.write(memory[3]);
        regWriteOut.write(memory[4]);
    }

    SC_CTOR(gd_ex_register) {
        SC_METHOD(read_write);
        sensitive << clock;
    }
};
