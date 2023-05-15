#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(forwarding_unit) {
    sc_in<REG_ADDR> ex_rs_in, ex_rt_in;
    sc_in<REG_ADDR> mem_rd_in, wb_rd_in;

    // CONTROL
    sc_in<bool> mem_reg_write, wb_reg_write;
    sc_out<int> ex_upper_alu_mux, ex_lower_alu_mux;

    int _ex_upper_alu_mux = 0;
    int _ex_lower_alu_mux = 0;

    void run() {
        REG_ADDR rs = ex_rs_in.read();
        REG_ADDR rt = ex_rt_in.read();

        std::cout << ex_rs_in.read() << " " << ex_rt_in.read() << " "
                  << wb_rd_in.read() << " " << mem_rd_in.read() << std::endl;

        if (mem_reg_write.read() && mem_rd_in.read() != 0) {
            if (rs == mem_rd_in.read())
                _ex_upper_alu_mux = 2;

            if (rt == mem_rd_in.read())
                _ex_lower_alu_mux = 2;
        }

        if (wb_reg_write.read() && wb_rd_in.read() != 0) {
            if (rs == wb_rd_in.read() && _ex_upper_alu_mux == 2)
                _ex_upper_alu_mux = 1;

            if (rt == wb_rd_in.read() && _ex_lower_alu_mux == 2)
                _ex_lower_alu_mux = 1;
        }

        ex_upper_alu_mux.write(_ex_upper_alu_mux);
        ex_lower_alu_mux.write(_ex_lower_alu_mux);
    }

    SC_CTOR(forwarding_unit) {
        SC_METHOD(run);
        sensitive << ex_rs_in << ex_rt_in << mem_rd_in << wb_rd_in
                  << mem_reg_write << wb_reg_write;
    }
};
