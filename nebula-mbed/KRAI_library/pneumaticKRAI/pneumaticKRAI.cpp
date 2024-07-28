#include "mbed.h"
#include "pneumaticKRAI.h"

pneumaticKRAI ::pneumaticKRAI(PinName pinPneu,
                              int posisi,
                              PneumaticPos pneumaticPos) : pinPneu_(pinPneu)
{
    pneuMaju = 1;
    pneuMundur = 0;
    if ((pneumaticPos == MAJU && posisi == 0) || (pneumaticPos == MUNDUR && posisi == 1))
    {
        pneuMaju = 0;
        pneuMundur = 1;
    }
}

void pneumaticKRAI::Contract(void)
{
    pinPneu_ = pneuMundur;
}
void pneumaticKRAI::Extend(void)
{
    pinPneu_ = pneuMaju;
}
