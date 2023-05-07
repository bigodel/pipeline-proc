#include <systemc.h>

SC_MODULE(program_counter) {
    sc_in_clk clock;
    sc_in<int> jump_address;
    sc_out<int> address;

    int current_address = 0;

    void increment() {
        int jump_addr = jump_address.read();

        if (jump_addr) {
            current_address = jump_addr;
        } else {
            current_address++;
        }

        address.write(current_address);
    }

    SC_CTOR(program_counter) {
        SC_METHOD(increment);
        sensitive << clock.pos();
    }
};
