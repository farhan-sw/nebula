#include "PID_KRAI.h"

PID::PID(float p, float i, float d, float _Ts,  float _max_integral, float _max_speed)
: ts(_Ts), max_integral(_max_integral), max_speed(_max_speed), kp(p), ki(i), kd(d)
{
    last_error = 0.0;
    integ = 0;
    deriv = 0;
    if(max_speed){
        vff = 1/max_speed;
    }
    else {
        vff = 0;
    }
}

void PID::setTunings(float p, float i, float d)
{
    kp = p;
    ki = i;
    kd = d;
}

void PID::setKp(float p){
    kp = p;
}

void PID::setKd(float d){
    kd = d;
}

float PID ::createpwm(float setpoint, float feedback, float max)
{
    error = setpoint - feedback;

    // get integral (bilinear transform)
    if (this->ki){
        integ += ki*(error + last_error) * ts / 2000000;
    }
    float prop = kp*error;
    if (fabs(max)>fabs(prop)){
        max_integral = fabs(max) - fabs(prop);
    }
    else{
        max_integral = 0.0;
    }
    if (fabs(integ) > fabs(max_integral))
        {
            integ = fabs(max_integral) * (fabs(integ) / integ);
        }
    // get derivatif with freq_lpf (bilinear transform)
    // y(n) = (2*(x(n)-x(n-1) - (y(n-1)(t-2N)))) / (t+2N)
    if (this->kd){
        deriv = kd*(error - last_error) / ts;
    }

    last_error = error;

    last_out = prop + integ + deriv + vff * setpoint;
    if (fabs(last_out) > fabs(max))
    {
        last_out = fabs(max) * (fabs(last_out) / last_out);
    }
    if(do_online_tuning){
        this->onlineTuning(setpoint, feedback);
    }
    return last_out;
}
void PID::onlineTuning(float setpoint, float feedback){
    if(abs(max_feedback)<abs(feedback)){
        max_feedback=feedback;
    }
    if (!rise_is_reached and abs(feedback) > 0.7*abs(setpoint)){
        rise_is_reached=1;
        rise_time = loop_counter*ts;
    }
    if(rise_is_reached){
        average_absolute_error += fabs(setpoint-feedback);
    }
    if (prevsp != 0 and prevsp != setpoint){
        float ess = fabs(feedback-prevsp)/fabs(prevsp);
        float overshoot = fabs((max_feedback-setpoint)/setpoint);
        if (ess > 0.1){
            ki = 1.5*ki;
        }
        else{
            if (overshoot > 0.2){
                ki = 0.8*ki;
                kp = 0.8*kp;
            }
            else if (rise_time > 2000 or float(rise_time)/prev_rise_time > 1.5 or !rise_is_reached){
                kp = 1.2*kp;
            }
        }
        //resetter
        max_feedback = 0;
        rise_time = 0;
        rise_is_reached = 0;
        loop_counter = 0;
    }
    prevsp = setpoint;
    loop_counter++;
}
void PID :: reset(){
    integ = 0;
    deriv = 0;
    last_error = 0;
    last_out = 0;
}