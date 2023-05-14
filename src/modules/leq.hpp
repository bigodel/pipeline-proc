#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(leq) {
    // input
    sc_in<WORD> a, b;

    // outputs
    sc_out<bool> result;

    void process() {
        if(a.read().to_int() <= b.read().to_int())
            result.write(true);
        else
            result.write(false);
    }

    SC_CTOR(leq) {
        SC_METHOD(process);
        sensitive << a << b;
    }
};
