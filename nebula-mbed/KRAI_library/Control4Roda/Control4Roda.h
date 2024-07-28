#ifndef _CONTROL_KRAI_H_
#define _CONTROL_KRAI_H_

// FOR RECTANGLE BASE
// #define COS45 0.70710678118
// #define COS14_53 -0.382807

#define max_pwm 0.8

// FOR SQUARE BASE
#define COS45 0.70710678118

#include "../odom2enc/Coordinate.h"
#include "../odom2enc/odom2enc.h"
#include "../Motor/Motor.h"
#include "../ControlMotor/ControlMotor.h"
#include "../encoderHAL/encoderHAL.h"
#include "../encoderKRAI/encoderKRAI.h"
#include "../StanleyPursuit/StanleyPursuit.h"
#include "../pidLo/pidLo.h"

class Control4Roda {
    protected:
        Coordinate last_pos;
        Coordinate speed_base;

        /* MOTOR */
        // pwm, fwd, rev
        Motor *FL_motor;
        Motor *FR_motor;
        Motor *BR_motor;
        Motor *BL_motor;

        /*Control Motor */
        ControlMotor *control_FL_motor;
        ControlMotor *control_FR_motor;
        ControlMotor *control_BR_motor;
        ControlMotor *control_BL_motor;

        /* ENCODER */
        encoderKRAI *encFL;
        encoderKRAI *encFR;
        encoderKRAI *encBR;
        encoderKRAI *encBL;

        /* ODOMETRI */
        // odometriKRAI *odom;
        odom2enc *odom;

        pidLo *vxPid;
        pidLo *vyPid;
        pidLo *wPid;

        /* TRAJECTORY FOLLOWER */
        /* odom2enc */
        const int total_destination = 1; // how  many target coordinate for automatic movement
        const int error = 1; // error automatization in cm
        const float max_pwm_velocity = 0.5;


        // Testing autopositioning menggunakan odom2enc
        float arr_x_test[3] = {0, 50, 0}; //cm
        float arr_y_test[3] = {100, 150, 100}; //cm

        // Testing autopositioning menggunakan odom2enc
        float arr_x_test2[1] = {0};
        float arr_y_test2[1] = {600};

        Coordinate initialPos;

        /*odom2enc*/
        StanleyPursuit *line;
        pidLo *pid;
        pidLo *pid2;


        int curr_dest_cout;
        int mode; // mode 1 pakai ke ball rack, mode 2 pakai ke SZ
        // Perlu 1 variabel lagi buat atas atau bawah

        float setpoint;
        float feedback;
        float max;
        float d_out;
        float v;

        /* INTERNAL VARIABLES */
        float vx_cmd, vy_cmd, w_cmd;
        float v_FL_curr, v_FR_curr, v_BR_curr, v_BL_curr;
        float FL_pwm, FR_pwm, BR_pwm, BL_pwm;
        float FL_target_speed, FR_target_speed, BR_target_speed, BL_target_speed;
        float vy_last, vx_last, w_last;
        float vy_motor, vx_motor, w_motor;

        /* Ultrasonic */
        float ultrasonic1;
        float ultrasonic2;

    public:
        Control4Roda(Motor *FL_motor, Motor *FR_motor, Motor *BR_motor, Motor *BL_motor, encoderKRAI *encFL, encoderKRAI *encFR, encoderKRAI *encBR, encoderKRAI *encBL, ControlMotor *control_FL_motor, ControlMotor *control_FR_motor, ControlMotor *control_BR_motor, ControlMotor *control_BL_motor, odom2enc *odom, pidLo *vxPid, pidLo *vyPid, pidLo *wPid, StanleyPursuit *line, pidLo *pid, pidLo *pid2);
        // Getter
        void getVars(float *vx_cmd, float *vy_cmd, float *w_cmd, float *v_FL_curr, float *v_FR_curr, float *v_BR_curr, float *v_BL_curr, float *FL_pwm, float *FR_pwm, float *BR_pwm, float *BL_pwm, float *FL_target_speed, float *FR_target_speed, float *BR_target_speed, float *BL_target_speed, float *vy_last, float *vx_last, float *w_last, float *vy_motor, float *vx_motor, float *w_motor,
                     float *x_pos, float *y_pos, float *theta_pos, float *x_dest, float *y_dest);


        // Setter
        void set_vx_cmd(float vx_cmd);
        void set_vy_cmd(float vy_cmd);
        void set_w_cmd(float w_cmd);


        // procedure
        void motorSamp();
        void forceBrakeSync();
        void pidMotorSamp();
        virtual void updatePosition();

        /* 
        * prosedur untuk melakukan sampling enc motor
        *
        *abstract method, akan dioverride tergantung child class (ada Constant spesific)*/
        virtual void encoderMotorSamp() = 0; 
        virtual void baseSpeed() = 0; 
        virtual void base() = 0;
};

#endif