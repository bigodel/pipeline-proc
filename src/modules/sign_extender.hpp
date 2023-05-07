#include "systemc.h"
#include "definitions.hpp"

SC_MODULE(sign_extender) {
    sc_in<IMM> input;
    sc_out<WORD> output;

    void extend() {
        // if the sign bit is set, extend with 1s
        if (input.read()[15] == 1)
            output.write(0b1111111100000000 | input.read());
        else // otherwise, extend with 0s
            output.write(input.read());
    }

    SC_CTOR(sign_extender) {
        SC_METHOD(extend);
        sensitive << input;
    }
};
