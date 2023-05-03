#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(mux3x1) {
    sc_in<bool> sel0, sel1;
    sc_in<WORD> a, b, c;
    sc_out<WORD> out;

    void select() {
        if (sel0.read() == 0 && sel1.read() == 0)
            out.write(a.read());
        else if (sel0.read() == 0 && sel1.read() == 1)
            out.write(b.read());
        else
            out.write(c.read());
    }

    SC_CTOR(mux3x1) {
        SC_METHOD(select);
        sensitive << sel0 << sel1 << a << b << c;
    }
};
