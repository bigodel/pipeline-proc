#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(reg_addr_mux) {
    sc_in<bool> sel;
    sc_in<REG_ADDR> a, b;
    sc_out<REG_ADDR> out;

    void select() {
        // return A if false, B if true
        if (!sel.read()) out.write(a.read());
        else out.write(b.read());
    }

    SC_CTOR(reg_addr_mux) {
        SC_METHOD(select);
        sensitive << sel << a << b;
    }
};
