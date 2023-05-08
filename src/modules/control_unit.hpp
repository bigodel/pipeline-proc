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

    void control() {}

    SC_CTOR(control_unit) {
        SC_METHOD(control);
    }
};
