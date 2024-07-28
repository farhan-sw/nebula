#ifndef ODOM3ENC_H
#define ODOM3ENC_H

#include <math.h>
#include "mbed.h"
#include "../encoderHAL/encoderHAL.h"
#include "Coordinate.h"

class odom3enc{
    
    private:
        encoderHAL *encL;
        encoderHAL *encR;
        encoderHAL *encAux;
        
    public:
        //referensi: https://www.youtube.com/watch?v=ZW7T6EFyYnc&ab_channel=Punchster2
        Coordinate position; 
        odom3enc(encoderHAL *encL, encoderHAL *encR, encoderHAL *encAux); 
        void updatePosition(void);
        void resetOdom(void);
        void setPosition(float x_input, float y_input, float theta_input);
};
#endif