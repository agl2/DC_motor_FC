#include "../systemc-2.3.1/src/systemc.h"
#include "../headers/sys_top.h"
#define T 2000
/**
* Para a simulacao depois de T ms.
*/
void sys_top::run() {

    cout << "@" << sc_time_stamp() <<" Starting simulation"<<endl;
    wait(T, SC_MS);
    cout << "@" << sc_time_stamp() <<" Terminating simulation"<<endl;
    sc_stop();
}

