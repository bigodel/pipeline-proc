#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(mux3x1) {
    sc_in<int> sel;
    sc_in<WORD> a, b, c;

    sc_out<WORD> out;

    void select() {
        // return A if 0, 
        // B if 1
        // C if 2
        switch (sel) {
            case 0:
                out.write(a.read());
                break;
            case 1:
                out.write(b.read());
                break;
            case 2:
                out.write(c.read());
                break;
        }
    }

    SC_CTOR(mux3x1) {
        SC_METHOD(select);
        sensitive << sel << a << b << c << sel;
    }
};
