#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(sign_extender) {
    sc_in<WORD> input;
    sc_out<WORD> output;

    void extend() {
        sc_bv<16> in = input.read().range(15, 0);
        WORD out;

        // if the sign bit is set, extend with 1s
        if (in[15] == 1) {
            sc_bv<16> ones = "1111111111111111";
            out = (ones, in);
        } else { // otherwise, extend with 0s
            sc_bv<16> zeroes = "0000000000000000";
            out = (zeroes, in);
        }

        output.write(out);
    }

    SC_CTOR(sign_extender) {
        SC_METHOD(extend);
        sensitive << input;
    }
};
