#ifndef _CONTROLMOTOR_H_
#define _CONTROLMOTOR_H_

#include "mbed.h"
#include "../pidLo/pidLo.h"
#include "../SMC_KRAI/SMC_KRAI.h"

class ControlMotor {
    private:
        pidLo *_address_pid;
        SMC *_address_smc;
        uint32_t _total_state;

        float _kp_pid_1;
        float _kp_pid_2;

        float _kd_pid_1;
        float _kd_pid_2;

        float _kp_smc_1;
        float _kp_smc_2;

        float _v_batas;

        uint32_t _state_curr;
    
    public:
        /** Create ControlMotor Interface
         * @param SMC
         * @param PID
         * 
         */
        ControlMotor(pidLo* address_pid, SMC* address_smc, float v_batas, float kp_pid_1, float kp_pid_2, float kd_pid_1, float kd_pid_2, float kp_smc_1, float kp_smc_2);
        float sgn(float in);
        float rumus_geser(float in, float vb, float k1, float k2);
        float createpwm(float setpoint, float feedback, float max);
};

#endif