#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    sc_in<int> start, term, opCode;
    sc_out<int> result;

    void process() {
        switch (opCode.read()) {
            case 0: // AND
                result.write(start.read() && term.read());
                break;
            case 1: // OR
                result.write(start.read() || term.read());
                break;
            case 2: // XOR
                result.write(start.read() ^ term.read());
                break;
            case 3: // NOT
                result.write(~start.read());
                break;
            case 4: // CMP
                result.write(start.read() == term.read());
                break;
            case 5: // ADD
                result.write(start.read() + term.read());
                break;
            case 6: // SUB
                result.write(start.read() - term.read());
                break;
        }
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << start << term << opCode;
    }
};
