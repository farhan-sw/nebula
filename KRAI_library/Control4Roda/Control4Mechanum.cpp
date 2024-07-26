#include "Control4Mechanum.h"

Control4Mechanum::Control4Mechanum(Motor *FL_motor, Motor *FR_motor, Motor *BR_motor, Motor *BL_motor, encoderKRAI *encFL, encoderKRAI *encFR, encoderKRAI *encBR, encoderKRAI *encBL, 
        ControlMotor *control_FL_motor, ControlMotor *control_FR_motor, ControlMotor *control_BR_motor, ControlMotor *control_BL_motor, odom2enc *odom, 
        pidLo *vxPid, pidLo *vyPid, pidLo *wPid, StanleyPursuit *line, pidLo *pid, pidLo *pid2): 
        Control4Roda(FL_motor, FR_motor, BR_motor, BL_motor, encFL, encFR, encBR, encBL, control_FL_motor, control_FR_motor, control_BR_motor, control_BL_motor, odom, vxPid, vyPid, wPid, line, pid, pid2){    
    this->line->setError(ERROR_THRESHOLD);
}

void Control4Mechanum::encoderMotorSamp(){
    this->baseSpeed();
    this->v_FL_curr = (float)this->encFL->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    this->v_FR_curr = (float)this->encFR->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    this->v_BR_curr = (float)this->encBR->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    this->v_BL_curr = (float)this->encBL->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    
    /* reset nilai encoder */
    this->encFL->reset();
    this->encFR->reset();
    this->encBR->reset();
    this->encBL->reset();
}

void Control4Mechanum::baseSpeed(){
    this->updatePosition();

    // printf("%f %f %f\t\t%d %f %f\n", this->odom->position.x, this->odom->position.y, this->odom->position.teta, curr_dest_cout, arr_x_offline_atas_1[curr_dest_cout], arr_y_offline_atas_1[curr_dest_cout]);
}

void Control4Mechanum::base() {

    // Robot jalannya lurus, ga perlu koreksi pake vc vy w PID
    this->vy_motor = this->vy_cmd;
    this->vx_motor = this->vx_cmd;
    this->w_motor = this->w_cmd;
    
    // Limiting the acceleration
    if(fabs(this->vy_motor - this->vy_last) > 0.015*MAX_ACCEL_Y){
        if (this->vy_last > this->vy_motor){
            this->vy_motor = this->vy_last - MAX_ACCEL_Y*0.015;
        } else {
            this->vy_motor = this->vy_last + MAX_ACCEL_Y*0.015;
        }
    }
    if(fabs(this->vx_motor - this->vx_last) > 0.015*MAX_ACCEL_X){
        if (this->vx_last > this->vx_motor){
            this->vx_motor = this->vx_last - MAX_ACCEL_X*0.015;    
        }
        else{
            this->vx_motor = this->vx_last + MAX_ACCEL_X*0.015;
        }
    }     
    this->vy_last = this->vy_motor;
    this->vx_last = this->vx_motor;

    // Initialize input variable for iK
    float vx_motor_input = this->vx_motor;
    float vy_motor_input = this->vy_motor;
    float w_motor_input  = this->w_motor;

    // this->FL_target_speed = vy_motor_input + vx_motor_input - w_motor_input * R_BASE;  
    // this->FR_target_speed = - vy_motor_input + vx_motor_input - w_motor_input * R_BASE;
    // this->BR_target_speed = - vy_motor_input - vx_motor_input - w_motor_input * R_BASE; 
    // this->BL_target_speed = vy_motor_input - vx_motor_input - w_motor_input * R_BASE; 

    this->FL_target_speed = (vy_motor_input + (vx_motor_input / COS45) + a * w_motor_input + b * w_motor_input) / 3;
    this->FR_target_speed = (vy_motor_input - (vx_motor_input / COS45) - a * w_motor_input - b * w_motor_input) / 3;
    this->BL_target_speed = (vy_motor_input - (vx_motor_input / COS45) + a * w_motor_input + b * w_motor_input) / 3; 
    this->BR_target_speed = (vy_motor_input + (vx_motor_input / COS45) - a * w_motor_input - b * w_motor_input) / 3;
}