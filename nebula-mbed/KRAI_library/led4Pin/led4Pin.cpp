#include "led4Pin.h"

LED4Pin::LED4Pin(PinName red, PinName green, PinName blue):
        _red(red), _green(green), _blue(blue) {

    // Set initial condition of PWM
    _red.period(0.0005);
    _red = 0;
    _green.period(0.0005);
    _green = 0;
    _blue.period(0.0005);
    _blue = 0;

}

void LED4Pin::period(float period){

    _red.period(period);
    _green.period(period);
    _blue.period(period);

}

void LED4Pin::setRGB(float red255, float green255, float blue255){
    _red = std::round ( (fabs(red255)/255.0)*10.0 ) / 10.0;
    _green = std::round ( (fabs(green255)/255.0)*10.0 ) / 10.0;
    _blue = std::round ( (fabs(blue255)/255.0)*10.0 ) / 10.0;
}

void LED4Pin::setColor(string strColor){
    // RED(255,0,0), GREEN(0,255,0), BLUE(0,0,255)
    // PURPLE(200,0,255), CYAN(0,255,255), YELLOW(255,255,0), ORANGE(255, 125, 0)

    if(strColor == "RED"){
        setRGB(255.0, 0, 0);
    } else if (strColor == "GREEN"){
        setRGB(0, 255.0, 0);
    } else if (strColor == "BLUE"){
        setRGB(0, 0, 255.0);
    } else if (strColor == "PURPLE"){
        setRGB(200.0, 0, 255.0);
    } else if (strColor == "CYAN"){
        setRGB(0, 255.0, 255.0);
    } else if (strColor == "YELLOW"){
        setRGB(255.0, 255.0, 0);
    } else if (strColor == "ORANGE"){
        setRGB(255.0, 125.0, 0);
    }
    
}

void LED4Pin::turnOff(){
    setRGB(0, 0, 0);
}