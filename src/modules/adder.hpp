#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(adder) {
    sc_in<WORD> a, b;
    sc_out<WORD> s;

    void add() { s.write(a.read() + b.read()); }

    SC_CTOR(adder) {
        SC_METHOD(add);
        sensitive << a << b;
    }
};
