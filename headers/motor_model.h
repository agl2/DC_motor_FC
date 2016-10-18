#ifndef MOTOR_MODEL_H_INCLUDED
#define MOTOR_MODEL_H_INCLUDED

#include "../systemc-2.3.1/src/systemc.h"

SC_MODULE (motor_model)
{
    /***PORTS*/
    sc_in<bool>                     clk_sig;
    sc_port<sc_fifo_in_if<double>>  i_power;
    sc_port<sc_fifo_out_if<double>> o_revolutions;

    /***VARIABLES*/
    double                          power;
    double                          t;
    double                          rpm;

    /***FUNCTIONS*/
    void                            motor();
    void                            read();
    //void                            write();


    SC_CTOR(motor_model) {
        power = 0.0;
        rpm = 0.0;
        SC_THREAD  (motor);
        SC_CTHREAD   (read, clk_sig.pos());
       // SC_CTHREAD   (write, clk_sig.pos());
    }
};


#endif // MOTOR_MODEL_H_INCLUDED
