#include "definitions.hpp"
#include <systemc.h>

SC_MODULE(control) {
    // input
    sc_in<WORD> instruction;

    // reg output
    sc_out<bool> reg_write;

    // alu output
    sc_out<ALU_OP> alu_op;

    // mem output
    sc_out<bool> mem_write, mem_read;

    // muxes output
    sc_out<bool> alu_src, reg_dst, mem_to_reg;

    void run() {
        // auxiliary values
        // EX
        ALU_OP _alu_op = 0;
        bool _alu_src = false;
        bool _reg_dst = false;
        // M
        bool _mem_write = false;
        bool _mem_read = false;
        // WB
        bool _reg_write = false;
        bool _mem_to_reg = false;

        WORD inst = instruction.read();
        OP opcode = inst.range(31, 26);
        switch (opcode.to_int()) {
            case OP_AND: // r-type instructions
            case OP_OR:
            case OP_XOR:
            case OP_NOT:
            case OP_CMP:
            case OP_ADD:
            case OP_SUB:
                // EX
                _alu_op = inst.range(5, 0);
                _reg_dst = true;
                // WB
                _reg_write = true;
                break;
            case OP_LD: // i-type instructions
                // EX
                _alu_src = true;
                // M
                _mem_read = true;
                // WB
                _reg_write = true;
                _mem_to_reg = true;
                std::cout << "LD " << _reg_write << std::endl;
                break;
            case OP_ST:
                // EX
                _alu_src = true;
                // M
                _mem_write = true;
                break;
            case OP_J: // j-type instructions
            case OP_JN:
            case OP_JZ:
            // // EX
            // alu_op.write();
            // alu_src.write();
            // reg_dst.write();
            // // M
            // mem_write.write();
            // mem_read.write();
            // // WB
            // reg_write.write();
            // mem_to_reg.write();
            default: 
                break;
        }
        // write values (if instruction is invalid it will write false)
        // EX
        alu_op.write(_alu_op);
        alu_src.write(_alu_src);
        reg_dst.write(_reg_dst);
        // M
        mem_write.write(_mem_write);
        mem_read.write(_mem_read);
        // WB
        reg_write.write(_reg_write);
        mem_to_reg.write(_mem_to_reg);
    }

    SC_CTOR(control) {
        SC_METHOD(run);
        sensitive << instruction;
    }
};
