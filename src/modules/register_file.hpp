#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(register_file) {
    // sc_in_clk clock;
    // inputs
    sc_in<WORD> reg1, reg2, write_reg;
    sc_in<WORD> write_data;

    // control signals
    sc_in<bool> reg_write;

    // output
    sc_out<WORD> data1, data2;

    WORD registers[REG_COUNT] = {0};

    void register_process() {
        //std::cout << reg1.read().to_string() << std::endl;
        //std::cout << reg1.read().range(25, 21).to_string() << std::endl;
        //std::cout << reg2.read().range(20, 16).to_string() << std::endl;
        //std::cout << write_reg.read().range(15, 11).to_string() << std::endl;

        data1.write(registers[reg1.read().range(25, 21).to_int()]);
        data2.write(registers[reg2.read().range(20, 16).to_int()]);

        if (reg_write.read() == true) {
            registers[write_reg.read().range(15, 11).to_int()] = write_data.read();
        }
    }

    SC_CTOR(register_file) {
        SC_METHOD(register_process);
        sensitive << reg1 << reg2 << write_reg << reg_write << write_data;
    }
};
