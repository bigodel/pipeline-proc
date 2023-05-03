#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(register_file) {
    sc_in<ADDR> reg1, reg2, write_reg;
    sc_in<WORD> write_data;
    sc_out<WORD> out_data1, out_data2;
    sc_in<bool> reg_write;

    WORD registers[REG_COUNT] = {0};

    void register_process() {
        out_data1.write(registers[reg1.read()]);
        out_data2.write(registers[reg2.read()]);

        if (reg_write.read() == true) {
            registers[write_reg.read()] = write_data.read();
        }
    }

    SC_CTOR(register_file) {
        SC_METHOD(register_process);
        sensitive << reg1 << reg2 << write_reg << reg_write << write_data;
    }
};
