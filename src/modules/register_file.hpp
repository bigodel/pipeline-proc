#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(register_file) {
    sc_in<ADDR> src1, src2, dest;
    sc_in<WORD> write_data;
    sc_out<WORD> reg1, reg2;
    sc_in<bool> reg_write;

    WORD registers[REG_COUNT] = {0};

    void register_process() {
        reg1.write(registers[src1.read()]);
        reg2.write(registers[src2.read()]);

        if (reg_write.read() == true) {
            registers[dest.read()] = write_data.read();
        }
    }

    SC_CTOR(register_file) {
        SC_METHOD(register_process);
        sensitive << src1 << src2 << dest << reg_write << write_data;
    }
};
