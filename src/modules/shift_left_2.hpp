#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(shift_left_2) {
    sc_in<WORD> in;
    sc_out<WORD> out;

    void shift() { out.write(in.read() << 2); }

    SC_CTOR(shift_left_2) {
        SC_METHOD(shift);
        sensitive << in;
    }
};
