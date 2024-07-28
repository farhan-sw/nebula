#include "ControlMotor.h"

ControlMotor::ControlMotor(pidLo* address_pid, SMC * address_smc, float v_batas, float kp_pid_1, float kp_pid_2,  float kd_pid_1, float kd_pid_2, float kp_smc_1, float kp_smc_2): _address_pid(address_pid), _address_smc(address_smc), _kp_pid_1(kp_pid_1), _kp_pid_2(kp_pid_2), _kd_pid_1(kd_pid_1), _kd_pid_2(kd_pid_2), _kp_smc_1(kp_smc_1), _kp_smc_2(kp_smc_2), _v_batas(v_batas)
{
    (*address_pid).setKp(this->_kp_pid_1);
    (*address_pid).setKd(this->_kd_pid_1);
    (*address_smc).setKp(this->_kp_smc_1);
}

// Fungsi Signum, bentuk seperti S, digunakan untuk
float ControlMotor::sgn(float in){
    return (in / (fabs(in) + 0.001));
}

// Untuk kecepatan beda, PID beda
// rumus_geser diperuntukkan agar mencari konstanta PID yang sesuai untuk kecepatan tertentu
float ControlMotor::rumus_geser(float in, float vb, float k1, float k2){
    return (this->sgn(fabs(in) - vb)* (k2-k1) + k2 + k1 )/2;
}

// Set komponen PID dan SMC berdasarkan state dan feedback
float ControlMotor::createpwm(float setpoint, float feedback, float max){
    (*_address_pid).setKp(rumus_geser(setpoint-feedback, _v_batas, _kp_pid_1, _kp_pid_2));
    (*_address_pid).setKd(rumus_geser(setpoint-feedback, _v_batas, _kd_pid_1, _kd_pid_2));
    (*_address_smc).setKp(rumus_geser(setpoint-feedback, _v_batas, _kp_smc_1, _kp_smc_2));

    float temp = (*_address_pid).createpwm(setpoint, feedback, max) + (*_address_smc).createpwm(setpoint, feedback, max);
    
    if (fabs(temp) > fabs(max)){
        temp = fabs(temp)*max/temp;
    }
    return temp;
}