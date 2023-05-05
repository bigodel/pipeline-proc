#include "systemc.h"

SC_MODULE(data_memory) {
    sc_in<int> regRead1, regRead2, regWrite, dataWrite;
    sc_out<int> dataRead1, dataRead2;

    int memory[32] = {0};

    void read() {
        dataRead1.write(memory[regRead1.read()]);
        dataRead2.write(memory[regRead2.read()]);
    }

    void write() {
        // Write the value in the memory 
        memory[regWrite.read()] = dataWrite.read();
    }

    SC_CTOR(data_memory) {
        SC_METHOD(read);
        sensitive << regRead1 << regRead2;

        SC_METHOD(write);
        sensitive << regWrite << dataWrite;
    }
};
