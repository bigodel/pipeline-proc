#include "systemc.h"
#include "definitions.hpp"
#include <iomanip>

using namespace std;

SC_MODULE(monitor_mux2x1) {
    sc_in<WORD> a, b, out;
    sc_in<bool> Clk;
    sc_in<bool> sel;

    void monitor() {
        cout << setw(10) << "TIME";
        cout << setw(5) << "a";
        cout << setw(5) << "b";
        cout << setw(5) << "sel";
        cout << setw(5) << "out" << endl;
        while(true) {
            cout << setw(10) << sc_time_stamp();
            cout << setw(5) << a.read();
            cout << setw(5) << b.read();
            cout << setw(5) << sel.read();
            cout << setw(5) << out.read() << endl;
            wait();
        }
    }

    SC_CTOR(monitor_mux2x1) {
        SC_THREAD(monitor);
        sensitive << Clk.pos();
    }
};
