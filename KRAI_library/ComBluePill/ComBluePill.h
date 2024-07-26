#ifndef COM_BLUE_PILL_H
#define COM_BLUE_PILL_H

#include "mbed.h"
#include "../Configurations/Variables.h"
#include "../Configurations/Constants.h"


class comBluePill{
    private:    
        BufferedSerial serial_port(BP_TX, BP_RX, 9600);
        serial_port.set_format(8, BufferedSerial::None, 1);

        int angle, pitch, roll;
        struct LIDAR{
            int dist;
            int str;
            int temp;
        };
        LIDAR lidar1;
        LIDAR lidar2;

    public:
        comBluePill(PinName tx, PinName rx): _serial(tx, rx){};
        LIDAR getLidar();
        
}


#endif