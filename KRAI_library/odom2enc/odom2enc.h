#ifndef ODOM2ENC_H
#define ODOM2ENC_H

#include "mbed.h"
#include "../encoderHAL/encoderHAL.h"
#include "../CMPS12_KRAI/CMPS12_KRAI.h"
#include "Coordinate.h"

class odom2enc{
    private:
        encoderHAL *encX;
        encoderHAL *encY;
    public:
        odom2enc(encoderHAL *encX, encoderHAL *encY);
        Coordinate position;
        void resetOdom(void);
        void updatePosition(void);
        void setPosition(float x_input, float y_input);
};

#endif