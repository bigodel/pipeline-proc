#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(program_counter) {
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<WORD> input;

    sc_out<WORD> output;

    WORD address;

    void increment() {
        if (reset.read() == 1)
            address = 0;
        else
            // If the input is word-aligned, update the current_address with the
            // input value
            if (input.read() % 4 == 0)
                address = input.read();

        // Output the current_address
        output.write(address);
    }

    SC_CTOR(program_counter) {
        // initialize address to 0
        address = 0;

        SC_METHOD(increment);
        sensitive << clk.pos() << reset.neg();
    }
};
