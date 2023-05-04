#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(data_memory) {
    sc_in<int> address;     // input: mem address
    sc_in<int> input_data;  // input: data to be written
    sc_in<bool> enable;      // input: write enable signal
    sc_out<int> data;       // output: input_data read from mem


    // The first value must always be zero
    int mem[10] = {0};

    void read_write() {
        if (enable)
            mem[address.read()] = input_data.read();
        else
            data.write(mem[address.read()]);
    }

    SC_CTOR(data_memory) {
        SC_METHOD(read_write);
        sensitive << address << input_data << enable;
    }
};
