#include "mbed.h"
#include "../../KRAI_library/Pinout/F407VET6_2023.h"
#include "../../KRAI_library/encoderKRAI/encoderKRAI.h"
#include "../../KRAI_library/Motor/Motor.h"
#include "../../KRAI_library/MovingAverage/MovingAverage.h"
#include "../../KRAI_library/ADRC_V2/ADRC_V2.h"


//======================================================================
// =============================== CAN BUS Setup========================
#include "../../KRAI_library/CanBusKRAI/BMAktuatorKRAI.hpp"
#include "../../KRAI_library/Pinout/BoardManagerV1.h"

uint32_t millis = 0;

CAN can(PA_11, PA_12,CAN_BAUD_RATE);

DigitalOut led(PA_1);

#define CAN_READ_TS         5
#define CAN_SEND_TS         10
#define CAN_PRINT_TS        1000

BMAktuatorKRAI master(22, &millis);

uint32_t prevTimerCAN = us_ticker_read();
uint32_t samplingCAN_MS = 5 * 1000; // mikroSecond
//======================================================================
// =========================== END CAN BUS Setup========================


// Printing through USB Cable
#define PRINTF_TX PA_9
#define PRINTF_RX PA_10
static BufferedSerial serial_port(PRINTF_TX, PRINTF_RX, 115200);
FileHandle *mbed::mbed_override_console(int fd) {
    return &serial_port;
}

/*
                  Motor Front (M_F)
                    /==========\
                   /            \
                  /              \
                 /================\
Motor Back Left (M_BL)        Motor Back Right (M_BR)
*/

bool isDebugEncoder = false;

// PIN Motor
// #define PWM_FRONT F407VET6_PWM_MOTOR_1
// #define FOR_FRONT F407VET6_FOR_MOTOR_1
// #define REV_FRONT F407VET6_REV_MOTOR_1

// #define PWM_BL F407VET6_PWM_MOTOR_2
// #define FOR_BL F407VET6_FOR_MOTOR_2
// #define REV_BL F407VET6_REV_MOTOR_2

// #define PWM_BR F407VET6_PWM_MOTOR_4
// #define FOR_BR F407VET6_FOR_MOTOR_4
// #define REV_BR F407VET6_REV_MOTOR_4



#define PWM_FRONT F407VET6_PWM_MOTOR_4
#define FOR_FRONT F407VET6_FOR_MOTOR_4
#define REV_FRONT F407VET6_REV_MOTOR_4

#define PWM_BL F407VET6_PWM_MOTOR_1
#define REV_BL F407VET6_REV_MOTOR_1
#define FOR_BL F407VET6_FOR_MOTOR_1

#define PWM_BR F407VET6_PWM_MOTOR_2
#define REV_BR F407VET6_REV_MOTOR_2
#define FOR_BR F407VET6_FOR_MOTOR_2



// // PIN Encoder
// #define CHA_FRONT F407VET6_ENCODER_1_1_A
// #define CHB_FRONT F407VET6_ENCODER_1_1_B

// #define CHA_BR F407VET6_ENCODER_1_2_B
// #define CHB_BR F407VET6_ENCODER_1_2_A

// #define CHA_BL F407VET6_ENCODER_1_3_A
// #define CHB_BL F407VET6_ENCODER_1_3_B



// PIN Encoder
#define CHA_FRONT F407VET6_ENCODER_1_2_B
#define CHB_FRONT F407VET6_ENCODER_1_2_A

#define CHA_BR F407VET6_ENCODER_1_3_A
#define CHB_BR F407VET6_ENCODER_1_3_B

#define CHA_BL F407VET6_ENCODER_1_1_A
#define CHB_BL F407VET6_ENCODER_1_1_B

// PPR
#define PPR_FRONT 537.6f
#define PPR_BL 537.6f
#define PPR_BR 537.6f

// Object Encoder
encoderKRAI encFront(CHA_FRONT, CHB_FRONT, PPR_FRONT, Encoding::X4_ENCODING);
encoderKRAI encBL(CHA_BL, CHB_BL, PPR_BL, Encoding::X4_ENCODING);
encoderKRAI encBR(CHA_BR, CHB_BR, PPR_BR, Encoding::X4_ENCODING);

// Object Motor
Motor motorFront(PWM_FRONT, FOR_FRONT, REV_FRONT);
Motor motorBL(PWM_BL, FOR_BL, REV_BL);
Motor motorBR(PWM_BR, FOR_BR, REV_BR);

// Object Moving Average
MovingAverage movAvg_speed_F(10);
MovingAverage movAvg_speed_BL(10);
MovingAverage movAvg_speed_BR(10);

// set timer
uint32_t prevTime = us_ticker_read();
uint32_t samplingADRC = 7 * 1000; // mikroSecond

// ====================== ADRC ===================
bool isDebugADRC = false;
// Parameter for normal speed
float b0_FrontMotor_normal = 430;
float tSettle_normal = 0.5;
float zESO_normal = 7.25;
float incrementInt_normal = 0.09;

float tSettle_Back = 0.5;
float zESO_Back = 7.25;
ADRC_V2 adrc_motor_F(samplingADRC / 1000000.0f, b0_FrontMotor_normal, tSettle_normal, zESO_normal, incrementInt_normal);
ADRC_V2 adrc_motor_BL(samplingADRC / 1000000.0f, b0_FrontMotor_normal, tSettle_Back, zESO_Back, incrementInt_normal);
ADRC_V2 adrc_motor_BR(samplingADRC / 1000000.0f, b0_FrontMotor_normal, tSettle_Back, zESO_Back, incrementInt_normal);


// ========================== INVERSE KINEMATICS ==========================
bool isDebugInverseKinematics = false;
// Konstanta
#define L 0.28867   
#define r 0.075
#define gamma 30.0 
float cosGamma = cos(gamma * M_PI / 180.0);
float sinGamma = sin(gamma * M_PI / 180.0);

void inverseKinematics(float Vy, float Vx, float W, float *wm_front, float *wm_BL, float *wm_BR){ 
    *wm_front = -Vx + (L * W);
    *wm_BL = sinGamma * Vx - cosGamma * Vy + (L * W);
    *wm_BR = sinGamma * Vx + cosGamma * Vy + (L * W);

}


// Initialisasi variabel kecepatan motor
float wm_front = 0;
float wm_BL = 0;
float wm_BR = 0;
float Vn = 0.0; // Kecepatan linier pada sumbu y
float V = 0.0;  // Kecepatan linier pada sumbu x
float W = 0.0;  // Kecepatan angular

float setPointFront = 0;
float setPointBL = 0;
float setPointBR = 0;
// ========================== END INVERSE KINEMATICS ==========================


int main()
{

    int prevPulses_F = 0;
    int temp_prevPulses_F = 0;
    int prevPulses_BL = 0;
    int temp_prevPulses_BL = 0;
    int prevPulses_BR = 0;
    int temp_prevPulses_BR = 0;

    float speed_F = 0;
    float speed_BL = 0;
    float speed_BR = 0;
    float pwm_F = 0;
    float pwm_BL = 0;
    float pwm_BR = 0;

    float setPoint = 2.5;
    float realsetPoint = 0;

    while(1) {
        // ====================== CAN BUS ===================
        millis = us_ticker_read() / 1000;
        
        if (us_ticker_read() - prevTimerCAN > samplingCAN_MS) {
            prevTimerCAN = us_ticker_read();
            if (master.readingCAN())
            {
                led = !led;
            }
        }

        // master.printData(100);

        if (us_ticker_read() - prevTime > samplingADRC) {
            // Proses Inverse Kinematics
            inverseKinematics(master.getMotor1()/1000.0f, master.getMotor2()/1000.0f, master.getInteger()/1000.0f, &wm_front, &wm_BL, &wm_BR);

            // realsetPoint = adrc_motor_F.fhan_setPointTrajectory(setPoint, 5);
            setPointFront = adrc_motor_F.fhan_setPointTrajectory(wm_front, 0);
            setPointBL = adrc_motor_BL.fhan_setPointTrajectory(wm_BL, 0);
            setPointBR = adrc_motor_BR.fhan_setPointTrajectory(wm_BR, 0);

            // Print Hasil Inverse Kinematics
            isDebugInverseKinematics = false;
            if(isDebugInverseKinematics)
            {
                printf("wm_front: %6f wm_BL: %6f wm_BR: %6f setPointFront: %6f setPointBL: %6f setPointBR: %6f Vn: %6f V: %6f W: %6f\n", wm_front, wm_BL, wm_BR, setPointFront, setPointBL, setPointBR, master.getMotor1()/1000.0f, master.getMotor2()/1000.0f, master.getInteger()/1000.0f);
            }
            
            prevTime = us_ticker_read();
            temp_prevPulses_F = encFront.getPulses();
            temp_prevPulses_BL = encBL.getPulses();
            temp_prevPulses_BR = encBR.getPulses();

            speed_F = movAvg_speed_F.movingAverage(float(temp_prevPulses_F - prevPulses_F) / (PPR_FRONT * (samplingADRC / 1000000.0f)));
            speed_BL = movAvg_speed_BL.movingAverage(float(temp_prevPulses_BL - prevPulses_BL) / (PPR_BL * (samplingADRC / 1000000.0f)));
            speed_BR = movAvg_speed_BR.movingAverage(float(temp_prevPulses_BR - prevPulses_BR) / (PPR_BR * (samplingADRC / 1000000.0f)));

            pwm_F = adrc_motor_F.createInputSignal(setPointFront, speed_F, 1.0f);
            pwm_BL = adrc_motor_BL.createInputSignal(setPointBL, speed_BL, 1.0f);
            pwm_BR = adrc_motor_BR.createInputSignal(setPointBR, speed_BR, 1.0f);

            prevPulses_F = temp_prevPulses_F;
            prevPulses_BL = temp_prevPulses_BL;
            prevPulses_BR = temp_prevPulses_BR;

            isDebugADRC = false;
            if(isDebugADRC)
            {
                // printf("F: %f BL: %f BR: %f ", pwm_F, pwm_BL, pwm_BR);
                printf("speed_F: %f speed_BL: %f speed_BR: %f\n", speed_F, speed_BL, speed_BR);
                // printf("F: %f BL: %f BR: %f\n", movAvg_pwm_F.movingAverage(pwm_F), movAvg_pwm_BL.movingAverage(pwm_BL), movAvg_pwm_BR.movingAverage(pwm_BR));
                // printf("F: %f BL: %f BR: %f\n", pwm_F, pwm_BL, pwm_BR);
            }

            isDebugEncoder = false;
            if(isDebugEncoder)
            {
                // printf("speed_F: %f speed_BL: %f speed_BR: %f", speed_F, speed_BL, speed_BR);
                printf("F: %d BL: %d BR: %d\n", temp_prevPulses_F, temp_prevPulses_BL, temp_prevPulses_BR);
            }


            // printf("setPoint: %f speed_F: %f speed_BL: %f speed_BR: %f\n", realsetPoint, speed_F, speed_BL, speed_BR);
            // printf("F: %f BL: %f BR: %f\n", movAvg_pwm_F.movingAverage(pwm_F), movAvg_pwm_BL.movingAverage(pwm_BL), movAvg_pwm_BR.movingAverage(pwm_BR));
            // printf("F: %f BL: %f BR: %f\n", pwm_F, pwm_BL, pwm_BR);
            // if (pwm_BL != 0 && pwm_BR != 0 && pwm_F != 0)
            // {
            //     printf("Front: %f BL: %f BR: %f\n", -adrc_motor_F.getDisturbance()/pwm_F, -adrc_motor_BL.getDisturbance()/pwm_BL, -adrc_motor_BR.getDisturbance()/pwm_BR);
            // }
        }

        motorFront.speed(pwm_F);
        motorBL.speed(pwm_BL);
        motorBR.speed(pwm_BR);
    }

    return 0;
}