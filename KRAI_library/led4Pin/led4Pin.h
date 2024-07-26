/*
    Library untuk mengontrol LED RGB Streep 4 pin (commmon anode)

 */

#ifndef MBED_LED4_RGB
#define MBED_LED4_RGB

#include "mbed.h"
#include <math.h>


class LED4Pin {
public:

    /** 
     *
     * @param red
     * @param green
     * @param blue
     * 
     */
    LED4Pin(PinName red, PinName green, PinName blue);
    
    /** Set the period of the pwm duty cycle.
     *
     * @param seconds - Pwm duty cycle in seconds.
     */
    void period(float period);
    
    /*
    FUNGSI setRGB(float red255, float green255, float blue255);
    Melakukan perubah warna LED RGB berdasarkan kode RGB skala 0-255
    */
    void setRGB(float red255, float green255, float blue255);

    /*
    FUNGSI setColor(string strColor);
    Melakukan perubahan warna dengan warna yang sudah di dekrasi, berikut list warna :
        - RED(255,0,0), GREEN(0,255,0), BLUE(0,0,255)
        - PURPLE(200,0,255), CYAN(0,255,255), YELLOW(255,255,0), ORANGE(255, 125, 0)
    */
    void setColor(string strColor);

    /*
    FUNGSI turnOff();
    Mematikan LED
    */
    void turnOff();

protected:
    PwmOut _red;
    PwmOut _green;
    PwmOut _blue;

};

#endif