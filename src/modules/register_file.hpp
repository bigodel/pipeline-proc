#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(register_file) {
    // sc_in_clk clock;
    // inputs
    sc_in<WORD> reg1, reg2;
    sc_in<REG_ADDR> write_reg;
    sc_in<WORD> write_data;

    // control signals
    sc_in<bool> reg_write;

    // output
    sc_out<WORD> data1, data2;

    WORD registers[REG_COUNT];

    void register_process() {
        int rs = reg1.read().range(25, 21).to_int();
        int rt = reg2.read().range(20, 16).to_int();

        data1.write(registers[rs]);
        data2.write(registers[rt]);

        if (reg_write.read() == true) {
            int rd = write_reg.read().to_int();
            registers[rd] = write_data.read();
        }
    }

    SC_CTOR(register_file) {
        for (int i = 0; i < REG_COUNT; ++i)
            registers[i] = i;

        SC_METHOD(register_process);
        sensitive << reg1 << reg2 << write_reg << reg_write << write_data;
    }
};
