#include "../systemc-2.3.1/src/systemc.h"
#include "../headers/motor_model.h"
#include <cmath>

void motor_model::read() {
    while(true)
    {
        power = i_power->read();
        wait();
    }
}

/**
* Simula o funcionamento do motor por meio de uma funcao de transferencia.
*/
void motor_model::motor()
{
    while(true)
    {
        t = sc_time_stamp().to_seconds();
        ///---JOGAR O POWER NA FORMULA PRA CONVERTER DE PWM para POTÊNCIA
        rpm = (0.0999001 + 0.0250219*std::exp(-9.9975*t) - 0.124922*std::exp(-2.0025*t))*power;
        o_revolutions->write(rpm);
        wait(1, SC_MS);
    }
}
