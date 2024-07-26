/*
 *  Author      : Garudago ITB 2021
 *  Developer   : Oktavianus Irvan Sitanggang EL18
 *                Garin Ichsan Nugraha STI18
 */
 
#ifndef _SMC_KRAI_H_
#define _SMC_KRAI_H_
 
#include "mbed.h"
 
class SMC {
    public :
        typedef enum Mode{
                POSISI,
                KECEPATAN
            }Mode;

        /** Create a SMC interface   
         *
         * @param kp konstant number for general 
         * @param ksigma konstant number for sigma t
         * @param epsilon konstant number for epsilon
         * @param beta konstant number for beta
         * @param time_sampling time sampling SMC
         * @param mode mode SMC = POSISI / KECEPATAN
         */
        SMC(float kp, float ksigma, float epsilon, float beta, float time_sampling, Mode mode);

         /** Return sign value  
         *
         * @param in input value
         */
        float sgn(float in);

        /** Return sat value  
         *
         * @param in input value
         */
        float sat(float in);


        /** Create a PWM output for motor  
         *
         * @param setpoint objective setpoint
         * @param feedback feedback error
         * @param saturate maximum pwm value
         */
        float createpwm( float setpoint , float feedback, float saturate);  


        /** Reset Integral
         * 
         * 
         * 
         */
        void reset();

        /** Change kp constant
         * 
         * @param kp new kp constant
         * 
         */
        void setKp(float kp);

    private :
        float _kp;
        float _sqrt_kp;
        float _k_integ;
        float _ksigma;
        float _epsilon;
        float _timeSampling;
        float _beta;
        Mode _mode;
        float prev_error;
        float curr_error;
        float diff_error;
        float prev_diff_error;
        float prev_sigma;
        float curr_sigma;
        float integ;
        float force;
        float smooth;
        float pwm;
        float saturate;
};

#endif