#include "definitions.hpp"
#include "systemc.h"

SC_MODULE(alu) {
    // input
    sc_in<WORD> a, b;

    // control signal
    sc_in<ALU_OP> alu_op; // switch case doesn't allow sc_uint

    // outputs
    sc_out<WORD> result;
    sc_out<bool> zero;

    void process() {
        WORD res;
        bool is_zero = false;

        if (alu_op.read() == ADD) {
            WORD add(a.read().to_int() + b.read().to_int());
            res = add;
            is_zero = res == 0;
        }
        else if (alu_op.read() == SUB) {
            //std::cout << a.read().to_int() << std::endl;
            //std::cout << b.read().to_int() << std::endl;
            //std::cout << a.read().to_int() - b.read().to_int() << std::endl;
            WORD sub(a.read().to_int() - b.read().to_int());
            res = sub;
            // NOTE: changed
            is_zero = res.to_int() < 0;
        }
        else if (alu_op.read() == AND) {
            res = a.read() & b.read();
            is_zero = res == 0;
        }
        else if (alu_op.read() == OR) {
            res = a.read() | b.read();
            is_zero = res == 0;
        }
        else if (alu_op.read() == XOR) {
            res = a.read() ^ b.read();
            is_zero = res == 0;
        }
        else if (alu_op.read() == NOT) {
            res = ~a.read();
            is_zero = res == 0;
        }
        else if (alu_op.read() == CMP) {
            res = a.read().to_int() < b.read().to_int();
            is_zero = res == 0;
        } 
        else {
            res = 0;
            is_zero = true;
        }

        result.write(res);
        zero.write(is_zero);
    }

    SC_CTOR(alu) {
        SC_METHOD(process);
        sensitive << a << b << alu_op;
    }
};
