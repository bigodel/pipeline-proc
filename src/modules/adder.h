#include "systemc.h"

SC_MODULE(adder) {
    sc_in<sc_uint<32>> A, B;
    sc_out<sc_uint<32>> S;

    void add () {
        S.write(A.read() + B.read());
    }

    SC_CTOR(adder) {
        SC_METHOD(add);
        sensitive << A << B;
    }
};
