#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(data_memory) {
    sc_in<WORD> address;     // input: memory address
    sc_in<WORD> input_data;  // input: data to be written
    sc_in<bool> enable;      // input: write enable signal
    sc_out<WORD> data;       // output: input_data read from memory

    WORD memory[4096] = {0}; // 4KB memory, initialized to 0

    void read_write() {
        if (enable)
            memory[address.read()] = input_data.read();

        data.write(memory[address.read()]);
    }

    SC_CTOR(data_memory) {
        SC_METHOD(read_write);
        sensitive << address << input_data << enable;
    }
};
