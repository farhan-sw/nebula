#pragma once 

#include "mbed.h"

/* Supply measurement chip (TI INA 219)
   http://www.ti.com/product/ina219
   
   The INA219 is a high-side current shunt and power monitor 
   with an I2C interface. The INA219 monitors both shunt drop
   and supply voltage, with programmable conversion times and 
   filtering. A programmable calibration value, combined with 
   an internal multiplier, enables direct readouts in amperes. 
   An additional multiplying register calculates power in watts. 
   The I2C interface features 16 programmable addresse
*/
class INA219
{
public: 
    INA219(PinName sda, PinName scl, 
            unsigned char adr /* range 0x80(1000000)-0x9E(1001111) */);
    INA219(PinName sda, PinName scl);
    bool detect(void);
    double getCurrent(void);
    double getVoltage(void);
protected:
    bool _det;
    unsigned char _adr;
    I2C _i2c;
};
