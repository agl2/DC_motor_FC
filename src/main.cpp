#include "../systemc-2.3.1/src/systemc.h"
#include "../headers/sys_top.h"
#include <cstdio>

sys_top* sys = NULL;

/**
* Instancia o 'top-level' do sistema.
*/
int sc_main(int argc, char* argv[])
{
    freopen("out.txt","w",stdout);
    sys = new sys_top("SYS");
    sc_start();

    return(0);
}
