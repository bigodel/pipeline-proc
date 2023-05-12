#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(fowarding_unity) {
    sc_in<REG_ADDR> ex_rs_in, ex_rt_in;

    sc_in<REG_ADDR> mem_rd_in, wb_rd_in;

    // CONTROL

    sc_in<bool> mem_reg_write, wb_reg_write;

    sc_out<int> ex_upper_alu_mux, ex_lower_alu_mux;

    int _ex_upper_alu_mux = 0;
    int _ex_lower_alu_mux = 0;

    void run() {
        std::cout << ex_rs_in.read() << " " << ex_rt_in.read()
                << " " << wb_rd_in.read() << " " << mem_rd_in.read() << std::endl;

        if (wb_reg_write.read() && wb_rd_in.read() != 0) {
            if (ex_rs_in.read() == wb_rd_in.read())
                _ex_upper_alu_mux = 1;

            if (ex_rt_in.read() == wb_rd_in.read())
                _ex_lower_alu_mux = 2;
        }

        if (mem_reg_write.read() && mem_rd_in.read() != 0) {
            if (ex_rs_in.read() == mem_rd_in.read())
                _ex_upper_alu_mux = 2;

            if (ex_rt_in.read() == mem_rd_in.read())
                _ex_lower_alu_mux = 1;
        }

        ex_upper_alu_mux.write(_ex_upper_alu_mux);
        ex_lower_alu_mux.write(_ex_lower_alu_mux);
    }

    SC_CTOR(fowarding_unity) {
        SC_METHOD(run);
        sensitive << ex_rs_in << ex_rt_in << mem_rd_in << wb_rd_in << mem_reg_write << wb_reg_write;
    }
};
