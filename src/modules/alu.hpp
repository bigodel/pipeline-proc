#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    sc_in<WORD> a, b;
    sc_in<int> command; // switch case doesn't allow sc_uint
    sc_out<WORD> result;

    void process() {
        switch (command.read()) {
        case ADD:
            result.write(a.read() + b.read());
            break;
        case SUB:
            result.write(a.read() - b.read());
            break;
        case AND:
            result.write(a.read() & b.read());
            break;
        case OR:
            result.write(a.read() | b.read());
            break;
        case XOR:
            result.write(a.read() ^ b.read());
            break;
        case NOT:
            result.write(~a.read());
            break;
        case CMP:
            result.write(a.read() < b.read());
            break;
        default:
            result.write(0);
            break;
        }
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << a << b << command;
    }
};
