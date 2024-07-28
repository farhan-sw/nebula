#include "SMC_KRAI.h"

SMC :: SMC(float kp, float ksigma, float epsilon, float beta, float time_sampling, Mode mode){
    _kp = kp;
    _ksigma = ksigma;
    _epsilon = epsilon;
    _beta = beta;
    _timeSampling = time_sampling;
    _mode = mode;
    _sqrt_kp = sqrt(fabs(_kp));
    _k_integ = 1.1*kp;
    integ = 0;
    prev_error = 0;
    prev_diff_error = 0;
    prev_sigma = 0;
}

float SMC :: sgn(float in){
    return  (in / (fabs(in) + _epsilon));
}

float SMC :: sat(float in){
    float val = in / _beta;
    if (val < -1) return -1;
    else if (val > 1) return 1;
    else return  val;
}

float SMC :: createpwm(float setpoint , float feedback, float saturate){
    curr_error = setpoint - feedback;
    diff_error = (curr_error - prev_error)/_timeSampling;

    if (_mode == KECEPATAN) curr_sigma = diff_error + _ksigma*curr_error;
    else if (_mode == POSISI) curr_sigma = ((diff_error - prev_diff_error)/_timeSampling) + 2 * _ksigma *diff_error + _ksigma*_ksigma * curr_error;
    integ += (sgn(prev_sigma) + sgn(curr_sigma))*_timeSampling/2;
 
    force =  _sqrt_kp * sqrt(fabs(curr_sigma)) * sgn(curr_sigma);
    smooth = _k_integ* integ;

    pwm = force + smooth;
 
    if (pwm >= saturate){
        pwm = saturate;
    }
    else if (pwm <= -saturate){
        pwm = -saturate;
    }

    prev_error = curr_error;
    prev_sigma = curr_sigma;
    prev_diff_error = diff_error;
    
    return pwm;
}

void SMC :: reset(){
    prev_error = 0;
    diff_error = 0;
    prev_diff_error = 0;
    prev_sigma = 0;
    integ = 0;
}

void SMC :: setKp(float kp){
    _kp = kp;
}