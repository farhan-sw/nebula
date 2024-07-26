#include "StanleyPursuit.h"
#include <cstdio>
#ifndef PI
#define PI 3.14159265358979
#endif

StanleyPursuit::StanleyPursuit()
{
    this->X0 = 0;
    this->Y0 = 0;
    this->error = 0;
    this->W = 0;
    this->t = 0;
}

void StanleyPursuit::setError(float error) {
    this->error = error;
}

float StanleyPursuit::getError() {
    return this->d;
}

float StanleyPursuit::getVi() {
    return this->vi;
}

float StanleyPursuit::getVj() {
    return this->vj;
}

float StanleyPursuit::getDi() {
    return this->di;
}

float StanleyPursuit::getDj() {
    return this->dj;
}

void StanleyPursuit::initialPosition(float x, float y, float w)
{
    this->X0 = x;
    this->Y0 = y;
    this->W = w;
}

void StanleyPursuit::setTarget(float x, float y)
{
    this->Xf = x;
    this->Yf = y;
    if (fabs(this->Xf - this->X0) <= 0.01){
        if (this->Yf > this->Y0){
            this->m = 999;
        }
        else {
            this->m = -999;
        }
    }
    else {
        this->m = (this->Yf - this->Y0) / (this->Xf - this->X0);
        if (this->m > 999){
            this->m = 999;
        }
        if (this->m < -999){
            this->m = -999;
        }
    }
}

void StanleyPursuit::updatePosition(float x, float y, float curr_theta, float *setpoint, float *feedback, float *max)
{
    this->theta = atan(this->m);
    if (this->Xf < this->X0){
        this->theta = this->theta + PI;
    }
    this->sinTheta = sin(this->theta);
    this->cosTheta = cos(this->theta);

    this->d = -1 * (this->Xf - x) * this->sinTheta + (this->Yf - y) * this->cosTheta;

    this->di = sinTheta;
    this->dj = cosTheta;

    this->t = (this->Xf - x) * this->cosTheta + (this->Yf - y) * this->sinTheta;
    // printf("%f\n", this->t);

    this->vi = this->cosTheta;
    this->vj = this->sinTheta;

    this->updateTheta(curr_theta, setpoint, feedback, max);
}

void StanleyPursuit::updateTheta(float theta_feed, float *setpoint, float *feedback, float *max)
{
    this->alreadySet = false;
    this->enterCondition = false;
    this->err1 = fabs(this->theta - theta_feed);
    this->err2 = 2 * PI - this->err1;

    if (this->theta > 0 && theta_feed < 0){
        this->enterCondition = true;
        if (this->err1 > this->err2){
            *setpoint = (-1)*theta;
            *feedback = (-1)*theta_feed - 2 * PI;
            *max = 1;
            this->alreadySet = true;
        }
    }
    else if (theta < 0 && theta_feed > 0){
        this->enterCondition = true;
        if (err1 > err2){
            *setpoint = (-1)*theta;
            *feedback = 2 * PI - theta_feed;
            *max = 1;
            this->alreadySet = true;
        }
    }
    else{
        this->enterCondition = true;
    }

    if (!alreadySet && enterCondition){
        *setpoint = theta;
        *feedback = theta_feed;
        *max = 1;
    }

    // W = (-1)*w_cmd;
}

void StanleyPursuit::setW(float W) {
    this->W = W;
}

bool StanleyPursuit::TargetReached(){
    // printf("%f\n", this->t);
    // printf("%f\n", this->error);
    if (this->t < this->error){
        return true;
    }
    else {
        return false;
    }
}


void StanleyPursuit::printVars(){
    printf("d = %f; t = %f; ", this->d, this->t);
}

float StanleyPursuit::getT(){
    return this->t;
}