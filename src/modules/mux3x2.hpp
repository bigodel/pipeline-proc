#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(mux3x2) {
    sc_in<bool> sel;
    sc_in<bool> a, b;
    sc_in<bool> c;

    sc_out<bool> out_a;
    sc_out<bool> out_b;

    void select() {
        if (!sel.read()){
            out_a.write(a.read());
            out_b.write(b.read());
        }
        else {
            out_a.write(c.read());
            out_b.write(c.read());
        }
    }

    SC_CTOR(mux3x2) {
        SC_METHOD(select);
        sensitive << sel << a << b << c;
    }
};
