#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    // input
    sc_in<WORD> a, b;

    // control signal
    sc_in<ALU_OP> alu_op; // switch case doesn't allow sc_uint

    // outputs
    sc_out<WORD> result;
    sc_out<bool> zero;

    void process() {
        WORD res;
        bool is_zero = false;

        switch (alu_op.read()) {
        case ADD:
            res = a.read() + b.read();
            is_zero = res == 0;
            break;
        case SUB:
            res = a.read() - b.read();
            is_zero = res == 0;
            break;
        case AND:
            res = a.read() & b.read();
            is_zero = res == 0;
            break;
        case OR:
            res = a.read() | b.read();
            is_zero = res == 0;
            break;
        case XOR:
            res = a.read() ^ b.read();
            is_zero = res == 0;
            break;
        case NOT:
            res = ~a.read();
            is_zero = res == 0;
            break;
        case CMP:
            res = a.read() < b.read();
            is_zero = res == 0;
            break;
        default:
            res = 0;
            is_zero = true;
            break;
        }

        result.write(res);
        zero.write(is_zero);
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << a << b << alu_op;
    }
};
