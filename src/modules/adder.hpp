#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(adder) {
    sc_in<WORD> a, b;
    sc_out<WORD> s;

    void add() { 
        WORD sum(a.read().to_int() + b.read().to_int());
        s.write(sum); 
    }

    SC_CTOR(adder) {
        SC_METHOD(add);
        sensitive << a << b;
    }
};
