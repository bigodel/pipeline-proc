#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    sc_in<int> regStart, regTerm;
    sc_in<int> opCode;
    sc_out<int> regDest;

    void process() {
        switch (opCode.read()) {
            case 0: // AND
                regDest.write(regStart.read() && regTerm.read());
                break;
            case 1: // OR
                regDest.write(regStart.read() || regTerm.read());
                break;
            case 2: // XOR
                regDest.write(regStart.read() ^ regTerm.read());
                break;
            case 3: // NOT
                regDest.write(~regStart.read());
                break;
            case 4: // CMP
                regDest.write(regStart.read() == regTerm.read());
                break;
            case 5: // ADD
                regDest.write(regStart.read() + regTerm.read());
                break;
            case 6: // SUB
                regDest.write(regStart.read() - regTerm.read());
                break;
        }
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << regStart << regTerm << opCode;
    }
};
