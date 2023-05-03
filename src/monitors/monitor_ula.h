#include "systemc.h"
#include <iomanip>

using namespace std;

SC_MODULE(monitor_ula) {
    sc_in<sc_uint<32>> A,B, RES;
    sc_in<int> CMD;
    sc_in<bool> Clk;

    void monitor() {
        cout << setw(10) << "Time";
        cout << setw(2) << "A";
        cout << setw(2) << "B";
        cout << setw(5) << "CMD";
        cout << setw(5) << "RES" << endl;
        while (true) {
            cout << setw(10) << sc_time_stamp();
            cout << setw(2) << A.read();
            cout << setw(2) << B.read();
            cout << setw(5) << CMD.read();
            cout << setw(5) << RES.read() << endl;
            wait();
        }
    }

    SC_CTOR(monitor_ula) {
        SC_THREAD(monitor);
        sensitive << Clk.pos();
    }
};
