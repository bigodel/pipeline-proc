#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    sc_in<WORD> A, B;
    sc_in<int> command; // switch case doesn't allow sc_uint
    sc_out<WORD> result;

    void process() {
        WORD a = A.read();
        WORD b = B.read();
        WORD output;

        switch (command.read()) {
        case 0b000: // AND
            output = a & b;
            break;
        case 0b001: // OR
            output = a | b;
            break;
        case 0b010: // ADD
            output = a + b;
            break;
        case 0b011: // SUB
            output = a - b;
            break;
        case 0b100: // SLT
            output = a < b;
            break;
        default:
            output = 0;
            break;
        }

        result.write(output);
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << A << B << command;
    }
};
