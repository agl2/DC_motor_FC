#ifndef FUZZY_CONTROLLER_H_INCLUDED
#define FUZZY_CONTROLLER_H_INCLUDED
#include "../fuzzylite/fl/Headers.h"
#include "../systemc-2.3.1/src/systemc.h"

using namespace fl;
using namespace std;


SC_MODULE (fuzzy_controller)
{
    /**PORTS**/
    sc_port<sc_fifo_out_if<double>>     o_power;
    sc_port<sc_fifo_in_if<double>>      i_revolutions;

    /**VARIABLES**/
    double                              revolutions;
    double                              freq_clock;
    double                              vReal = 0;
    double                              vRef = 30;
    double                              errorValue = 0;
    double                              errorChangeValue = 0;
    double                              lastError = 0;
    double                              power = 0;
    double                              lastPower = 0;
    double                              motorInput = 0;

    /**FUZZYLITE**/
    Engine*                             engine;
    InputVariable*                      error;
    InputVariable*                      changeError;
    OutputVariable*                     changePower;
    RuleBlock*                          ruleblock;
    string                              status;


    /**FUNCTIONS**/
    void                                read();
    void                                do_job();
    void                                initialize();

    SC_CTOR(fuzzy_controller) {
        revolutions = 0.0;
        initialize();
        SC_THREAD(read);
        SC_THREAD(do_job);
    }
};

#endif // FUZZY_CONTROLLER_H_INCLUDED
