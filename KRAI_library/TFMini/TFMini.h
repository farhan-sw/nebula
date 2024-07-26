#ifndef TFMINI_H
#define TFMINI_H

#include "../mbed.h"

// Pinout List
#define tfmini_SDA PB_3
#define tfmini_SCL PB_10

// Address List
#define tfmini1_ADD 0x10 // default address

// Register List
#define ADD_REG         0x0026


class TFmini{
    
    I2C* i2c;
    int  i2cAddress;
    
    
    public:
        TFmini(PinName TF_SDA, PinName TF_SCL, char address);
        
//        float getTheta();
//        float getX();
//        float getY();
//        int getMode();
        
        void olahData();
        
        void changeAddress(char new_add);
        void reset_tf();
        
        void findAddress();
        
        int dist_HL;
        int strength_HL;
        int mode_tf;
        
    private:
        char data[7];
};

#endif /* TFMINI_H */