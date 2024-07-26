#include "INA219.h"

INA219::INA219(PinName sda, PinName scl, unsigned char adr) :
    _i2c(sda, scl)
{
    _i2c.frequency(100000);
    _adr = adr;
}

INA219::INA219(PinName sda, PinName scl) :
    _i2c(sda, scl)
{
    _i2c.frequency(100000);
    _adr = 0x80;
}

bool INA219::detect(void)
{
    _i2c.start();
    bool det = (1 == _i2c.write(_adr|1/*write address for reading*/));
    _i2c.stop();
    return det;
}

double INA219::getCurrent(void)
{
    double d = 0.0;
    char r = 0x01;
    if (0 == _i2c.write(_adr, &r, sizeof(r), true))
    {
        char v[2] = {0};
        if (0 == _i2c.read(_adr, v, sizeof(v)))
        {
            int u = (int)((((short)v[0]) << 8) + (unsigned char)v[1]) * 10; // uV
            int i = u * 2/*0.5ohm*/;
            d = 1e-6 * i;
        }
    }
    else 
    {
        _i2c.stop();
    }
    return d;
}

double INA219::getVoltage(void)
{
    double d = 0.0;
    char r = 0x02;
    if (0 == _i2c.write(_adr, &r, sizeof(r), true))
    {
        char v[2] = {0};
        if (0 == _i2c.read(_adr, v, sizeof(v)))
        {
            int u = (int)(((((short)v[0]) << 8) + (unsigned char)v[1]) >> 3) * 4; // mV
            d = 1e-3 * u;
        }
    }
    else 
        _i2c.stop();
    return d;
}
