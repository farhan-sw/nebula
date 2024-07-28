#ifndef _F446RE_SLAVE_2022_H_
#define _F446RE_SLAVE_2022_H_

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

#define F446RE_SLAVE_PWM_MOTOR_A PB_14
#define F446RE_SLAVE_FOR_MOTOR_A PB_13 // PA_10
#define F446RE_SLAVE_REV_MOTOR_A PA_10 // PB_13

#define F446RE_SLAVE_PWM_MOTOR_B PA_15
#define F446RE_SLAVE_FOR_MOTOR_B PA_13 // PA_14
#define F446RE_SLAVE_REV_MOTOR_B PA_14 // PA_13

#define F446RE_SLAVE_PWM_MOTOR_C PC_9
#define F446RE_SLAVE_FOR_MOTOR_C PA_11 // PB_12
#define F446RE_SLAVE_REV_MOTOR_C PB_12 // PA_11

#define F446RE_SLAVE_PWM_MOTOR_D PB_0
#define F446RE_SLAVE_FOR_MOTOR_D PC_3 // PC_2
#define F446RE_SLAVE_REV_MOTOR_D PC_2 // PC_3

#define F446RE_SLAVE_PWM_MOTOR_E PB_1
#define F446RE_SLAVE_FOR_MOTOR_E PB_15 // PB_10
#define F446RE_SLAVE_REV_MOTOR_E PB_10 // PB_15

/******** ENCODERS *******/
/* FORMAT
#define <BOARD>_ENCODER_<N>_<M>_<A/B> <PIN> // <PIN_KITAB_SAKTI>
N : 1/2
M : A/B/C/D
*/

/* C DAN D DI BUKU SAKTI TERBALIK, YANG DI BAWAH SUDAH DIBALIK */

#define F446RE_SLAVE_ENCODER_1_A_A PC_1
#define F446RE_SLAVE_ENCODER_1_A_B PC_15
#define F446RE_SLAVE_ENCODER_1_B_A PC_13
#define F446RE_SLAVE_ENCODER_1_B_B PC_10
#define F446RE_SLAVE_ENCODER_1_C_A PC_11 // PC_12
#define F446RE_SLAVE_ENCODER_1_C_B PC_12 // PC_11
#define F446RE_SLAVE_ENCODER_1_D_A PC_14 // PC_0
#define F446RE_SLAVE_ENCODER_1_D_B PC_0 // PC_14

#define F446RE_SLAVE_ENCODER_2_A_A PB_3
#define F446RE_SLAVE_ENCODER_2_A_B PB_5
#define F446RE_SLAVE_ENCODER_2_B_A PB_4
#define F446RE_SLAVE_ENCODER_2_B_B PA_8
#define F446RE_SLAVE_ENCODER_2_C_A PA_9 // PB_2
#define F446RE_SLAVE_ENCODER_2_C_B PB_2 // PA_9
#define F446RE_SLAVE_ENCODER_2_D_A PC_7 // PB_6
#define F446RE_SLAVE_ENCODER_2_D_B PB_6 // PC_7

/************* UART *************/
#define F446RE_SLAVE_UART_RX PA_1
#define F446RE_SLAVE_UART_TX PA_0

/************* I2C /************/
/* FORMAT
#define <BOARD>_I2C_<SDA/SCL> <PIN_TERTULIS_DI_BOARD> // <PIN_KITAB_SAKTI>
*/

#define F446RE_SLAVE_I2C_SDA PB_9 // PC_9
#define F446RE_SLAVE_I2C_SCL PB_8 // PA_8

/************** SPI **************/
#define F446RE_SLAVE_SPI_MISO PA_6
#define F446RE_SLAVE_SPI_MOSI PA_7
#define F446RE_SLAVE_SPI_SCK PA_5
#define F446RE_SLAVE_SPI_CS PA_4

/************ I/O Pin ************/
#define F446RE_SLAVE_GPIO_1 PC_4
#define F446RE_SLAVE_GPIO_2 PB_7
#define F446RE_SLAVE_GPIO_3 PD_2
#define F446RE_SLAVE_GPIO_4 PH_0
#define F446RE_SLAVE_GPIO_5 PA_12
#define F446RE_SLAVE_GPIO_6 PH_1
#define F446RE_SLAVE_GPIO_7 PC_5

#define F446RE_SLAVE_ANALOG_1 PC_8
#define F446RE_SLAVE_ANALOG_2 PC_6

#endif
