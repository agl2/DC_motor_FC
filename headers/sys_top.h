#ifndef SYS_TOP_H_INCLUDED
#define SYS_TOP_H_INCLUDED

#include "../systemc-2.3.1/src/systemc.h"
#include "../headers/fuzzy_controller.h"
#include "../headers/motor_model.h"

SC_MODULE (sys_top)
{
    /**MODULES**/
    fuzzy_controller *fuzzy_c;
    motor_model *motor_m;

    /**VARIABLES**/
    sc_fifo <double> speed;
    sc_fifo <double> power;

    /**SIGNALS**/
    sc_clock clk_sig;
    sc_signal< bool > rst;

    void run();

    SC_CTOR(sys_top) : clk_sig ("clk_sig", 10, SC_NS) {

        motor_m = new motor_model ("MM");
        fuzzy_c = new fuzzy_controller ("FC");

        motor_m->clk_sig        (clk_sig);
        motor_m->i_power       (power);
        motor_m->o_revolutions (speed);

        fuzzy_c->o_power      (power);
        fuzzy_c->i_revolutions(speed);

        SC_THREAD (run);
    }

    ~sys_top() {
        delete motor_m;
        delete fuzzy_c;
    }
};

#endif // SYS_TOP_H_INCLUDED
