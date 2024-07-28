#ifndef _F446RE_MASTER_2022_H_
#define _F446RE_MASTER_2022_H_

/*
CATATAN: 
Forward --> Clockwise
Reverse --> Counter Colockwise
                               __________________________________
Clockwise/Counter     ________|          |                    |  |  
Dilihat dari sini -> |________           |                    |  |
                              |__________|____________________|__| 
                      ^-Shaft   ^-Gear          ^-Motor         ^-Encoder
                          
Positive --> Aturan Tangan Kanan ke Kanan
*/

/******** MOTORS *******/
/* FORMAT
#define <BOARD>_MOTOR_<N>     <PWM_PIN>
#define <BOARD>_FOR_MOTOR_<N> <FORWARD_PIN> // <FORWARD_PIN_KITAB_SAKTI>
#define <BOARD>_REV_MOTOR_<N> <REVERSE_PIN> // <REVERSE_PIN_KITAB_SAKTI>
*/

#define F446RE_MASTER_PWM_MOTOR_FR PB_8
#define F446RE_MASTER_FOR_MOTOR_FR PB_3 
#define F446RE_MASTER_REV_MOTOR_FR PA_14 

#define F446RE_MASTER_PWM_MOTOR_FL PB_9
#define F446RE_MASTER_FOR_MOTOR_FL PC_3 
#define F446RE_MASTER_REV_MOTOR_FL PC_2 

#define F446RE_MASTER_PWM_MOTOR_BR PB_10
#define F446RE_MASTER_FOR_MOTOR_BR PC_11 
#define F446RE_MASTER_REV_MOTOR_BR PC_10 

#define F446RE_MASTER_PWM_MOTOR_BL PA_11 // SALAH SATU BOARD GA JALAN
#define F446RE_MASTER_FOR_MOTOR_BL PC_14 // SALAH SATU BOARD GA JALAN
#define F446RE_MASTER_REV_MOTOR_BL PC_15 // SALAH SATU BOARD GA JALAN

#define F446RE_MASTER_PWM_MOTOR_C PB_0
#define F446RE_MASTER_FOR_MOTOR_C PC_13 
#define F446RE_MASTER_REV_MOTOR_C PB_1 

/******** ENCODERS *******/
/* FORMAT
#define <BOARD>_ENCODER_<N>_<M>_<A/B> <PIN> // <PIN_KITAB_SAKTI>
N : 1/2
M : A/B/C/D
*/

/* C DAN D DI BUKU SAKTI TERBALIK, YANG DI BAWAH SUDAH DIBALIK */

#define F446RE_MASTER_ENCODER_A_A_A PC_4
#define F446RE_MASTER_ENCODER_A_A_B PC_5
#define F446RE_MASTER_ENCODER_A_B_A PB_2 
#define F446RE_MASTER_ENCODER_A_B_B PB_14
#define F446RE_MASTER_ENCODER_A_C_A PA_13
#define F446RE_MASTER_ENCODER_A_C_B PA_12
#define F446RE_MASTER_ENCODER_A_D_A PB_15
#define F446RE_MASTER_ENCODER_A_D_B PC_8




#define F446RE_MASTER_ENCODER_B_A_A PB_3
#define F446RE_MASTER_ENCODER_B_A_B PB_5
#define F446RE_MASTER_ENCODER_B_B_A PB_4
#define F446RE_MASTER_ENCODER_B_B_B PA_8
#define F446RE_MASTER_ENCODER_B_C_A PA_9 // PB_2
#define F446RE_MASTER_ENCODER_B_C_B PB_2 // PA_9
#define F446RE_MASTER_ENCODER_B_D_A PC_7 // PB_6
#define F446RE_MASTER_ENCODER_B_D_B PB_6 // PC_7

// /************* UART *************/
// #define F446RE_MASTER_UART_RX PA_1
// #define F446RE_MASTER_UART_TX PA_0

// /************* I2C /************/
// /* FORMAT
// #define <BOARD>_I2C_<SDA/SCL> <PIN_TERTULIS_DI_BOARD> // <PIN_KITAB_SAKTI>
// */

#define F446RE_MASTER_I2C_SDA PB_9 // PC_9
#define F446RE_MASTER_I2C_SCL PB_8 // PA_8

// /************** SPI **************/
// #define F446RE_MASTER_SPI_MISO PA_6
// #define F446RE_MASTER_SPI_MOSI PA_7
// #define F446RE_MASTER_SPI_SCK PA_5
// #define F446RE_MASTER_SPI_CS PA_4

// /************ I/O Pin ************/
// #define F446RE_MASTER_GPIO_1 PC_4
// #define F446RE_MASTER_GPIO_2 PB_7
// #define F446RE_MASTER_GPIO_3 PD_2
// #define F446RE_MASTER_GPIO_4 PH_0
// #define F446RE_MASTER_GPIO_5 PA_12
// #define F446RE_MASTER_GPIO_6 PH_1
// #define F446RE_MASTER_GPIO_7 PC_5

// #define F446RE_MASTER_ANALOG_1 PC_8
// #define F446RE_MASTER_ANALOG_2 PC_6

#endif
