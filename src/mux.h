#include "defines.h"
#include <systemc.h>

SC_MODULE(mux2x1) {
    sc_in<bool> sel;
    sc_in<WORD_TYPE> a, b, out;

    SC_CTOR(mux2x1) {
        SC_METHOD(select);
        sensitive << sel << a << b;
    }

    void select() {
        if (sel.read()) out.write(a.read()) 
        else out.write(b.read());
    }   
}

SC_MODULE(mux3x1) {
    
    SC_CTOR(mux3x1) {

    }
}
