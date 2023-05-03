#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(mux2x1) {
    sc_in<bool> sel;
    sc_in<WORD> a, b;
    sc_out<WORD> out;

    void select() {
        if (sel.read()) out.write(a.read());
        else out.write(b.read());
    }

    SC_CTOR(mux2x1) {
        SC_METHOD(select);
        sensitive << sel << a << b;
    }
};
