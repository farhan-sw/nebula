#ifndef TFPLUS_H
#define TFPLUS_H

#include "../mbed.h"
// #include <string.h>

#define tfplus_SDA PC_9
#define tfplus_SCL PA_8

#define tfplus1_ADD 0x12 // default address 0x10
#define tfplus2_ADD 0x14 // default address 0x10

#define tfplus_TX   PA_0
#define tfplus_RX   PA_1

// #define I2C_REG     (char[]){0x5A, 0x05, 0x0A, 0x01, 0x6A}
// #define CHANGE_ADD1 (char[]){0x5A, 0x05, 0x0B, tfplus1_ADD, 0x71}
// #define CHANGE_ADD2 (char[]){0x5A, 0x05, 0x0B, tfplus2_ADD, 0x73}



class TFplus{
    
    I2C* i2c;
    int  i2cAddress;
    
    
    public:
        TFplus(PinName TF_SDA, PinName TF_SCL, char address);
        
        void olahData();
        
        void changeAddress(char new_add);
        void reset();
        
        void findAddress();
        
        int dist_HL;
        int strength_HL;
        int temp_HL;
        
    private:
        char data[9];
};

#endif /* TFPLUS_H */