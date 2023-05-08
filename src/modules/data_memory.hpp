#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(data_memory) {
    // input
    sc_in<WORD> address;
    sc_in<WORD> write_data;

    // control signals
    sc_in<bool> mem_write, mem_read;

    // output
    sc_out<WORD> data;

    // actual memory
    WORD memory[1024] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    void mem_access() {
        // TODO: check if we need to divide by 4
        if (mem_read.read())
            // TODO: check if range is ok
            data.write(memory[address.read().range(5, 0).to_int()]);
        if (mem_write.read())
            memory[address.read().to_int()] = write_data.read();
    }

    SC_CTOR(data_memory) {
        SC_METHOD(mem_access);
        sensitive << address << write_data << data;
    }
};
