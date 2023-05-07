#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(control_unit) {
    // pc output
    sc_out<bool> pc_src;

    // reg output
    sc_out<bool> reg_write, reg_dst;

    // alu output
    sc_out<ALU_OP> alu_op;
    sc_out<bool> alu_src;

    // mem output
    sc_out<bool> mem_write, mem_read, mem_to_reg;

    void control() {}

    SC_CTOR(control_unit) {
        SC_METHOD(control);
    }
};
