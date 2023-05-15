#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(detection_unity) {
    sc_in<WORD> if_id_reg_rs;
    sc_in<WORD> if_id_reg_rt;
    sc_in<WORD> id_ex_reg_rt;

    // ## CONTROL ##
    sc_in<bool> branch;
    sc_in<bool> id_ex_mem_read;

    // Force nop ("no operation") instruction into EX stage on next clock cycle
    sc_out<bool> control_mux_nop;

    // Hold instructions in ID and IF stages for one clock cycle
    sc_out<bool> pc_write;
    sc_out<bool> if_id_write;

    void run() {
        // # Auxiliate #
        REG_ADDR _if_id_reg_rs = if_id_reg_rs.read().range(25, 21);
        REG_ADDR _if_id_reg_rt = if_id_reg_rt.read().range(20, 16);
        REG_ADDR _id_ex_reg_rt = id_ex_reg_rt.read().range(20, 16);

        bool _control_mux_nop = false;
        bool _pc_write = true;
        bool _if_id_write = true;

        if (branch) {
            _if_id_write = false;
        }

        // Instruction in EX is a load
        if (id_ex_mem_read) {
            // Instruction in ID has a source register that matches
            // the load destination register
            if (_id_ex_reg_rt == _if_id_reg_rs ||
                _id_ex_reg_rt == _if_id_reg_rt) {

                // Force nop (“no operation”) instruction into
                // EX stage on next clock cycle
                _control_mux_nop = true;

                // Hold instructions in ID and IF stages for one clock cycle
                _pc_write = _if_id_write = false;
            }
        }

        control_mux_nop.write(_control_mux_nop);
        pc_write.write(_pc_write);
        if_id_write.write(_if_id_write);
    }

    SC_CTOR(detection_unity)
        : if_id_reg_rs("if_id_reg_rs"), if_id_reg_rt("if_id_reg_rt"),
          id_ex_reg_rt("id_ex_reg_rt"), id_ex_mem_read("id_ex_mem_read") {
        SC_METHOD(run);
        sensitive << if_id_reg_rs << if_id_reg_rt << id_ex_reg_rt
                  << id_ex_mem_read;
    }
};
