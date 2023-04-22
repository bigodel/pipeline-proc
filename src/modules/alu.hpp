#include "systemc.h"

SC_MODULE(alu) {
    sc_in<sc_uint<32>> A, B;
    sc_in<int> command; // switch case don't allow sc_uint
    sc_out<sc_uint<32>> result;
    // sc_out<bool> ZERO;

    void process() {
        switch (command) {
        case 0: // AND
            result.write(A.read() && B.read());
            break;
        case 1: // OR
            result.write(A.read() || B.read());
            break;
        case 2: // ADD
            result.write(A.read() + B.read());
            break;
        case 3: // SUB
            result.write(A.read() - B.read());
            break;
        default: // Set-on-less-than
            result.write(A.read() < B.read());
            break;
        }
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << A << B << command;
    }
};
