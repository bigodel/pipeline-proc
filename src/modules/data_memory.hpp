#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(data_memory) {
    // IMPORTANT: I'm treating data memory as data and register memory also

    sc_in<int> regStart, regTerm, regDest, result;
    sc_in<bool> isReading;
    sc_out<int> dataStart, dataTerm;

    // The first value must always be zero
    int mem[10] = {0};

    void read_write() {
        if (isReading.read()) {
            dataStart.write(mem[regStart.read()]);
            dataTerm.write(mem[regTerm.read()]);
        }
        else
            mem[regDest.read()] = result.read();
    }

    SC_CTOR(data_memory) {
        SC_METHOD(read_write);
        sensitive << regStart << regTerm << regDest << result << isReading;
    }
};
