#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(program_counter) {
    sc_in_clk clock;
    //sc_in<bool> reset;
    sc_out<int> address;

    int addressCounter = -1;

    // TODO: this should break, but it dosen't
    void increment() {
        if(addressCounter < INST_COUNT) {
            address.write(addressCounter); // Putting to read -1 index in a array
            addressCounter++;
        }
    }

    SC_CTOR(program_counter) {
        SC_METHOD(increment);
        sensitive << clock;
        //SC_METHOD(reset);
        //sensitive << reset;
    }
};
