#ifndef _F407VET6_2023_PINOUT_H_
#define _F407VET6_2023_PINOUT_H_

/*
CATATAN:
Forward --> Clockwise
Reverse --> Counter Colockwise
                               __________________________________
Clockwise/Counter     ________|          |                    |  |
Dilihat dari sini -> |________           |                    |  |
                              |__________|____________________|__|
                      ^-Shaft   ^-Gear          ^-Motor         ^-Encoder
*/

#define F407VET6_PWM_MOTOR_1 PB_13
#define F407VET6_FOR_MOTOR_1 PC_5
#define F407VET6_REV_MOTOR_1 PD_15

#define F407VET6_PWM_MOTOR_2 PB_14
#define F407VET6_FOR_MOTOR_2 PD_1
#define F407VET6_REV_MOTOR_2 PB_12

#define F407VET6_PWM_MOTOR_3 PC_7
#define F407VET6_FOR_MOTOR_3 PA_12
#define F407VET6_REV_MOTOR_3 PA_11

#define F407VET6_PWM_MOTOR_4 PA_15
#define F407VET6_FOR_MOTOR_4 PC_11 // PA_12
#define F407VET6_REV_MOTOR_4 PC_10

#define F407VET6_PWM_MOTOR_5 PB_8
#define F407VET6_FOR_MOTOR_5 PE_0
#define F407VET6_REV_MOTOR_5 PE_3

#define F407VET6_PWM_MOTOR_6 PB_9
#define F407VET6_FOR_MOTOR_6 PE_1
#define F407VET6_REV_MOTOR_6 PB_5

#define F407VET6_PWM_MOTOR_7 PC_6
#define F407VET6_FOR_MOTOR_7 PE_13
#define F407VET6_REV_MOTOR_7 PE_12

#define F407VET6_PWM_MOTOR_8 PB_7
#define F407VET6_FOR_MOTOR_8 PD_6
#define F407VET6_REV_MOTOR_8 PD_5

#define F407VET6_PWM_MOTOR_9 PB_10
#define F407VET6_FOR_MOTOR_9 PE_15
#define F407VET6_REV_MOTOR_9 PE_10

#define F407VET6_PWM_MOTOR_10 PA_2
#define F407VET6_FOR_MOTOR_10 PC_1
#define F407VET6_REV_MOTOR_10 PC_0

#define F407VET6_PWM_MOTOR_11 PA_3
#define F407VET6_FOR_MOTOR_11 PC_3
#define F407VET6_REV_MOTOR_11 PC_2

#define F407VET6_ENCODER_1_1_A PE_9
#define F407VET6_ENCODER_1_1_B PC_8
#define F407VET6_ENCODER_1_2_A PD_7
#define F407VET6_ENCODER_1_2_B PE_6
#define F407VET6_ENCODER_1_3_A PE_2 // PB_2
#define F407VET6_ENCODER_1_3_B PB_3
#define F407VET6_ENCODER_1_4_A PD_4
#define F407VET6_ENCODER_1_4_B PE_5

#define F407VET6_ENCODER_2_1_A PB_15 // TIM 1
#define F407VET6_ENCODER_2_1_B PE_14 
#define F407VET6_ENCODER_2_2_A PD_13  // TIM 4
#define F407VET6_ENCODER_2_2_B PD_12
#define F407VET6_ENCODER_2_3_A PB_0 // TIM 3
#define F407VET6_ENCODER_2_3_B PB_1
#define F407VET6_ENCODER_2_4_A PD_10
#define F407VET6_ENCODER_2_4_B PE_11

#define F407VET6_UART_TX_1 PA_9
#define F407VET6_UART_RX_1 PA_10
#define F407VET6_UART_TX_2 PA_2 // Jangan digunakan bersamaan dengan motor 10
#define F407VET6_UART_RX_2 PA_3 // Jangan digunakan bersamaan dengan motor 11

#define F407VET6_I2C_SDA_1 PB_7 // Jangan digunakan bersamaan dengan motor 8 //pwm
#define F407VET6_I2C_SCL_1 PB_6
#define F407VET6_I2C_SDA_2 PB_11
#define F407VET6_I2C_SCL_2 PB_10 // Jangan digunakan bersamaan dengan motor 9 //pwm
#define F407VET6_I2C_SDA_3 PC_9
#define F407VET6_I2C_SCL_3 PA_8

#define F407VET6_SPI_MOSI PA_7 //pwm //led2
#define F407VET6_SPI_MISO PA_6 //pwm
#define F407VET6_SPI_SCK PA_5 //pwm
#define F407VET6_SPI_CS PA_4

/*Belum Di Cek*/
#define F407VET6_DIGITAL_1_1 PD_8 
#define F407VET6_DIGITAL_1_2 PD_9
#define F407VET6_DIGITAL_1_3 PD_11 
#define F407VET6_DIGITAL_1_4 PD_14 
#define F407VET6_DIGITAL_1_5 PE_2
#define F407VET6_DIGITAL_1_6 PE_7

/*Belum Di Cek*/
#define F407VET6_DIGITAL_2_1 PD_0
#define F407VET6_DIGITAL_2_2 PD_2
#define F407VET6_DIGITAL_2_3 PD_3
#define F407VET6_DIGITAL_2_4 PE_4
#define F407VET6_DIGITAL_2_5 PC_12
#define F407VET6_DIGITAL_2_6 PC_13

/*Belum Di Cek*/
#define F407VET6_ANALOG_1 PA_0 //pwm
#define F407VET6_ANALOG_2 PA_1 //pwm
#define F407VET6_ANALOG_3 PC_4

#endif
