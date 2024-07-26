#ifndef _PID_KRAI_H_
#define _PID_KRAI_H_

#include "math.h"
#include "mbed.h"

class PID {
    // private:
    public:
        float kp;
        float ki;
        float kd;
        float ts;
        float max_integral;
        float max_speed;
        float vff;

        float error;
        float integ;
        float deriv;

        float last_error;
        float last_out;

        //var for tuning
        float prevsp=0;
        float prevkp;
        float prevki;
        float max_feedback;
        int rise_is_reached=0;
        int loop_counter = 0;
        int rise_time;
        int prev_rise_time = 0;
        float average_absolute_error =0 ;
    
        PID(float p, float i, float d, float _Ts, float _maxIntegral, float maxSpeed);
        float createpwm(float setpoint, float feedback, float max);
        void setTunings(float p, float i, float d);
        void setKp(float p);
        void setKd(float d);
        void reset();
        int do_online_tuning = 0;
        void onlineTuning(float setpoint, float feedback);
};

#endif