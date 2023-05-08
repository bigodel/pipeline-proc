#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(mux) {
    sc_in<bool> sel;
    sc_in<WORD> a, b;
    sc_out<WORD> out;

    void select() {
        // return A if false, B if true
        if (!sel.read()) out.write(a.read());
        else out.write(b.read());
    }

    SC_CTOR(mux) {
        SC_METHOD(select);
        sensitive << sel << a << b;
    }
};
