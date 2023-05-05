#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(data_memory) {
    // IMPORTANT: I'm treating data memory as data and regIndexister memory also

    sc_in<int> regIndexStart, regIndexTerm, regIndexDest, result;
    sc_out<int> regValueStart, regValueTerm;

    // The first value must always be zero
    int mem[10] = {0};

    void read() {
        regValueStart.write(mem[regIndexStart.read()]);
        regValueTerm.write(mem[regIndexTerm.read()]);
    }

    void write() {
        mem[regIndexDest.read()] = result.read();
    }

    SC_CTOR(data_memory) {
        SC_METHOD(read);
        sensitive << regIndexStart << regIndexTerm;

        SC_METHOD(write);
        sensitive << regIndexDest << result;
    }
};
