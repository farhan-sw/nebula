#ifndef PNEUMATICKRAI_H
#define PNEUMATICKRAI_H

#include "mbed.h"

class pneumaticKRAI {

public:

    typedef enum PneumaticPos {

        MAJU,
        MUNDUR

    } PneumaticPos;

    pneumaticKRAI(PinName pinPneu, int posisi = 0,PneumaticPos pneumaticPos = MUNDUR);  // posisi pneumatik 1

    void Extend(void);
    /**
     * Extend digunakan untuk memanjangkan Peumatik
     *
     **/
    void Contract(void);
    /**
     * Contaract digunakan untuk memendekkan Pneumatik
     *
     **/
private:

    PneumaticPos pneumaticPos_ ;

    DigitalOut pinPneu_ ;

    int pneuMaju;
    int pneuMundur;
};

#endif
