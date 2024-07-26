#include "TFPlus.h"
#include <math.h>

// Serial pc2 (USBTX, USBRX, 115200);
char RESET_PLUS_REG[4] =   {0x5A, 0x04, 0x10, 0x6E};
char SAVE_PLUS_REG[4] =    {0x5A, 0x04, 0x11, 0x6F};
char OBTAIN_CM_PLUS[5] =   {0x5A, 0x05, 0x00, 0x01, 0x60};

TFplus::TFplus(PinName TF_SDA, PinName TF_SCL, char address){
    
    i2c = new I2C(TF_SDA, TF_SCL);
    i2c->frequency(100);
    i2cAddress = address << 1;
    
    dist_HL = 0;
    strength_HL = 0;
    temp_HL = 0;
}


void TFplus::olahData(){
    i2c->write(i2cAddress, OBTAIN_CM_PLUS, sizeof(OBTAIN_CM_PLUS));
    i2c->read(i2cAddress, data, 7, false);
    
    dist_HL = (data[3]) << 8 | (data[2]);
    strength_HL = (data[5]) << 8 | (data[4]);
    temp_HL = (data[7] << 8 | data[6]);
}


void TFplus::changeAddress(char new_add){
    char sum = 0x5A + 0x05 + 0x08 + new_add;
    char change_reg[5] = {0x5A, 0x05, 0x0B, new_add, sum};

    i2c->write(i2cAddress, change_reg, 5); // change address
    i2cAddress = new_add << 1;
    i2c->write(i2cAddress, SAVE_PLUS_REG, sizeof(SAVE_PLUS_REG)); // saving
}

void TFplus::reset(){
    i2c->write(i2cAddress, RESET_PLUS_REG, sizeof(RESET_PLUS_REG));
    i2cAddress = 0x10;
    i2c->write(i2cAddress, SAVE_PLUS_REG, sizeof(SAVE_PLUS_REG)); // saving
}

void TFplus::findAddress(){
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