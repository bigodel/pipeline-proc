#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(control_unit) {
    // input
    sc_in<WORD> inst;

    // reg output
    sc_out<bool> reg_write;

    // alu output
    sc_out<ALU_OP> alu_op;

    // mem output
    sc_out<bool> mem_write, mem_read;

    // muxes output
    sc_out<bool> pc_src, alu_src, reg_dst, mem_to_reg;

    void control() {
        OP opcode = inst.read().range(31, 26);
        switch (opcode.to_int()) {
        case 0:                 // r-type instructions
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:

            break;

        case 7:                 // i-type instructions
        case 8:

            break;
        case 9:                 // j-type instructions
        case 10:
        case 11:

            break;

        default:                // invalid instruction

            break;
        }
    }

    SC_CTOR(control_unit) { SC_METHOD(control); }
};
