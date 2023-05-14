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
    WORD memory[MEM_COUNT] = {0};

    void mem_access() {
        if (mem_read.read())
            data.write(memory[address.read().range(5, 0).to_int()]);
        if (mem_write.read())
            memory[address.read().to_int()] = write_data.read();
    }

    SC_CTOR(data_memory) {
        for (int i = 0; i < MEM_COUNT; ++i)
            memory[i] = i;

        SC_METHOD(mem_access);
        sensitive << address << write_data << data;
    }
};
