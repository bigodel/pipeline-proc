#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    sc_in<WORD> A, B;
    sc_in<int> command; // switch case doesn't allow sc_uint
    sc_out<WORD> result;
    sc_out<bool> zero;

    void process() {
        WORD a = A.read();
        WORD b = B.read();
        bool is_zero;
        WORD output;

        switch (command.read()) {
        case 0b000: // AND
            output = a & b;
            is_zero = output == 0;
            break;
        case 0b001: // OR
            output = a | b;
            is_zero = output == 0;
            break;
        case 0b010: // ADD
            output = a + b;
            is_zero = output == 0;
            break;
        case 0b011: // SUB
            output = a - b;
            is_zero = output == 0;
            break;
        case 0b100: // SLT
            output = a < b;
            is_zero = output == 0;
            break;
        default:
            output = 0;
            is_zero = true;
            break;
        }

        result.write(output);
        zero.write(is_zero);
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << A << B << command;
    }
};
