#include "TFMini.h"
#include <math.h>

// Serial pc (USBTX, USBRX, 115200);

//const char DIST_REG[3] = {0x01, 0x02, 0x07};
//const char RESET_REG[2] = {0x00, 0x06};

char DIST_REG      [3] =     {0x01, 0x02, 0x07};
char RESET_MINI_REG[2] =     {0x00, 0x06};
char LIMIT_DISABLED[2] =     {0x0055, 0x00};
char LIMIT_ENABLED [2] =     {0x0055, 0x01};
char LONG_MODE     [2] =     {0x0050, 0x07};

TFmini::TFmini(PinName TF_SDA, PinName TF_SCL, char address){
    
    i2c = new I2C(TF_SDA, TF_SCL);
    i2c->frequency(100);
    i2cAddress = address << 1;
    i2c->write(i2cAddress, LIMIT_DISABLED, sizeof(LIMIT_DISABLED));
    i2c->write(i2cAddress, LONG_MODE, sizeof(LONG_MODE));
    
    dist_HL = 0;
    strength_HL = 0;
    mode_tf = 0;
}


void TFmini::olahData(){
    i2c->write(i2cAddress, DIST_REG, 3, true);
    i2c->read(i2cAddress, data, 7, false);
    
    dist_HL = (data[3]) << 8 | (data[2]);
    strength_HL = (data[5]) << 8 | (data[4]);
    mode_tf = data[6];
}



// ***************************************************
// Public Function :

//int TFmini::getDistance(){
//    olahData();
//    dist_HL = (data[3]) << 8 | (data[2]);
//    
//    return dist_HL;
//}
//
//int TFmini::getStrength(){
//    olahData();
//    strength_HL = (data[5]) << 8 | (data[4]);
//    
//    return strength_HL;
//}
//
//int TFmini::getMode(){
//    olahData();
//    mode_tf = data[6];
//    
//    return mode_tf;
//}

void TFmini::changeAddress(char new_add){
    char reg_change[2] = {ADD_REG, new_add << 1}; 
    i2c->write(i2cAddress, reg_change, 2);
    i2cAddress = new_add << 1;
}

void TFmini::reset_tf(){
    i2c->write(i2cAddress, RESET_MINI_REG, 2);
    i2cAddress = 0x10;
}

void TFmini::findAddress(){
    printf("I2CU! Searching for I2C devices...\n");

    int count = 0;
    for (int address=0; address<256; address+=2) {
        if (!i2c->write(address, NULL, 0)) { // 0 returned is ok
            printf(" - I2C device found at address 0x%02X\n", address);
            count++;
        }
    }
    printf("%d devices found\n", count);
}

// ***************************************************