#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(adder) {
    sc_in<WORD> A, B;
    sc_out<WORD> S;

    void add() { S.write(A.read() + B.read()); }

    SC_CTOR(adder) {
        SC_METHOD(add);
        sensitive << A << B;
    }
};
