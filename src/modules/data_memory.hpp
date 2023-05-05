#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(data_memory) {
    // IMPORTANT: I'm treating data memory as data and register memory also

    sc_in<int> regStart, regTerm, regDest, result;
    sc_out<int> dataStart, dataTerm;

    // The first value must always be zero
    int mem[10] = {0};

    void read() {
        dataStart.write(mem[regStart.read()]);
        dataTerm.write(mem[regTerm.read()]);
    }

    void write() {
        mem[regDest.read()] = result.read();
    }

    SC_CTOR(data_memory) {
        SC_METHOD(read);
        sensitive << regStart << regTerm;

        SC_METHOD(write);
        sensitive << regDest << result;
    }
};
