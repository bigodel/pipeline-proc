#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    sc_in<int> dataRead1, dataRead2, opCode;
    sc_out<int> dataWrite;

    void process() {
        switch (opCode.read()) {
            case 0: // AND
                dataWrite.write(dataRead1.read() && dataRead2.read());
                break;
            case 1: // OR
                dataWrite.write(dataRead1.read() || dataRead2.read());
                break;
            case 2: // XOR
                dataWrite.write(dataRead1.read() ^ dataRead2.read());
                break;
            case 3: // NOT
                dataWrite.write(~dataRead1.read());
                break;
            case 4: // CMP
                dataWrite.write(dataRead1.read() == dataRead2.read());
                break;
            case 5: // ADD
                dataWrite.write(dataRead1.read() + dataRead2.read());
                break;
            case 6: // SUB
                dataWrite.write(dataRead1.read() - dataRead2.read());
                break;
        }
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << dataRead1 << dataRead2 << opCode;
    }
};
