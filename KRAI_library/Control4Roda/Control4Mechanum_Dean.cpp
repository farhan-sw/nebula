#include "Control4Mechanum_Dean.h"

ControlMecanum::ControlMecanum(Motor *FL_motor, Motor *FR_motor, Motor *BR_motor, Motor *BL_motor, Motor *C_motor, encoderKRAI *encC, ControlMotor *control_C_motor) {
    this->FL_motor = FL_motor;
    this->FR_motor = FR_motor;
    this->BR_motor = BR_motor;
    this->BL_motor = BL_motor;
    this->C_motor = C_motor;
    this->encC = encC;
    this->control_C_motor = control_C_motor;
    this->otomatis = false;
}

void ControlMecanum::set_vx_cmd(float vx_cmd) {
    this->vx_cmd = vx_cmd;
}

void ControlMecanum::set_vy_cmd(float vy_cmd) {
    this->vy_cmd = vy_cmd;
}

void ControlMecanum::set_w_cmd(float w_cmd) {
    this->w_cmd = w_cmd;
}

void ControlMecanum::set_C_target_speed(float C_target_speed) {
    this->C_target_speed = C_target_speed;
}

void ControlMecanum::reverseOtomatis(bool otomatis) {
    this->otomatis = otomatis;
}

void ControlMecanum::motorSamp() {
    this->FL_motor->speed(this->FL_pwm);
    this->FR_motor->speed(this->FR_pwm);
    this->BR_motor->speed(this->BR_pwm); 
    this->BL_motor->speed(this->BL_pwm);
    this->C_motor->speed(this->C_pwm);
}

void ControlMecanum::encoderMotorSamp(){
    // this->baseSpeed();
    this->v_C_curr = (float)this->encC->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    // this->v_FR_curr = (float)this->encFR->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    // this->v_BR_curr = (float)this->encBR->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    // this->v_BL_curr = (float)this->encBL->getPulses()*2*PI*WHEEL_RAD*S_TO_US/(ENC_MOTOR_PULSE*ENC_MOTOR_SAMP_US_DEF);
    
    /* reset nilai encoder */
    //this->encC->reset();
    // this->encFR->reset();
    // this->encBR->reset();
    // this->encBL->reset();
}

void ControlMecanum::pidMotorSamp(){
    /* menghitung pid motor base */
    this->C_pwm = this->control_C_motor->createpwm(this->C_target_speed, this->v_C_curr, max_pwm);
    // this->FR_pwm = this->control_FR_motor->createpwm(this->FR_target_speed, this->v_FR_curr, max_pwm);
    // this->BR_pwm = this->control_BR_motor->createpwm(this->BR_target_speed, this->v_BR_curr, max_pwm);
    // this->BL_pwm = this->control_BL_motor->createpwm(this->BL_target_speed, this->v_BL_curr, max_pwm);
}

void ControlMecanum::updatePosition(){
    // this->odom->updatePosition();

    // printf("%d\n", this->mode);

    // printf("%f %f %f\t\t%d %d %f %f\n", this->odom->position.x, this->odom->position.y, this->odom->position.teta, mode, curr_dest_cout, arr_x_offline_atas_1[curr_dest_cout], arr_y_offline_atas_1[curr_dest_cout]);

    // if (this->otomatis) {
    //     if (this->line->TargetReached() == true && this->mode < 3) {
    //         // printf("HAAAAALLLLLLLLLOOOOOOOOOOOO\n");
    //         this->line->initialPosition(this->odom->position.x, this->odom->position.y, this->odom->position.teta);
    //         if (mode == 1) {
    //             this->line->setTarget((float)(this->initialPos.x + this->arr_x_offline_atas_1[this->curr_dest_cout]), (float)(this->initialPos.y + this->arr_y_offline_atas_1[this->curr_dest_cout]));
    //         }
    //         else if (mode == 2) {
    //             this->line->setTarget((float)(this->initialPos.x + this->arr_x_offline_atas_2[this->curr_dest_cout]), (float)(this->initialPos.y + this->arr_y_offline_atas_2[this->curr_dest_cout]));
    //         }
    //         this->curr_dest_cout++;
    //         if (this->curr_dest_cout == 3 && this->mode == 1) {
    //             // printf("masuk\n");
    //             this->mode = 2;
    //             this->curr_dest_cout = 1;
    //         }
    //         else if (this->curr_dest_cout == 3 && this->mode == 2) {
    //             this->mode = 3;
    //         }
    //     }
    // }
}

void ControlMecanum::baseSpeed(){
    // this->updatePosition();

    // printf("%f %f %f\t\t%d %f %f\n", this->odom->position.x, this->odom->position.y, this->odom->position.teta, curr_dest_cout, arr_x_offline_atas_1[curr_dest_cout], arr_y_offline_atas_1[curr_dest_cout]);

    // if (this->otomatis) {
    //     this->speed_base.x = (this->odom->position.x - this->last_pos.x)*(S_TO_US/SAMP_BASE_SPEED_US);
    //     this->last_pos.x = this->odom->position.x;
        
    //     this->speed_base.y = (this->odom->position.y - this->last_pos.y)*(S_TO_US/SAMP_BASE_SPEED_US);
    //     this->last_pos.y = this->odom->position.y;
        
    //     this->last_pos.teta = this->odom->position.teta;
    // }
}

void ControlMecanum::base4Mecanum() {
    if (this->otomatis) {
        // this->line->updatePosition(this->odom->position.x, this->odom->position.y, this->odom->position.teta, &this->setpoint, &this->feedback, &this->max);
        // this->d_out = this->pid->createpwm(0, this->line->getError(), 0.5);
        // this->vx_motor = this->v * this->line->getVi() + this->d_out * this->line->getDi();
        // this->vy_motor = this->v * this->line->getVj() + this->d_out * this->line->getDj();
        // this->w_motor = this->wPid->createpwm(this->setpoint, this->feedback, this->max);
        // this->line->setW((-1) * this->w_motor);
    }
    else {
        // Robot jalannya lurus, ga perlu koreksi pake vc vy w PID
        // this->vy_motor = this->vyPid->createpwm(this->vy_cmd, this->speed_base.y, 1);
        // this->vx_motor = this->vxPid->createpwm(this->vx_cmd, this->speed_base.x, 1);
        // this->w_motor = this->wPid->createpwm(this->w_cmd, this->speed_base.teta, 1);
        // this->vy_motor = this->vy_cmd;
        // this->vx_motor = this->vx_cmd;
        // this->w_motor = this->w_cmd;
    }
    
    // if(fabs(this->vy_motor - this->vy_last) > MAX_ACCEL_Y){
    //     if (this->vy_last > this->vy_motor){
    //         this->vy_motor = this->vy_last - MAX_ACCEL_Y;
    //     } else {
    //         this->vy_motor = this->vy_last + MAX_ACCEL_Y;
    //     }
    // }
    // if(fabs(this->vx_motor - this->vx_last) > MAX_ACCEL_X){
    //     if (this->vx_last > this->vx_motor){
    //         this->vx_motor = this->vx_last - MAX_ACCEL_X;    
    //     }
    //     else{
    //         this->vx_motor = this->vx_last + MAX_ACCEL_X;
    //     }
    // }     
    // this->vy_last = this->vy_motor;
    // this->vx_last = this->vx_motor;

    float vx_motor_input = this->vx_cmd;
    float vy_motor_input = this->vy_cmd;
    float w_motor_input  = this->w_cmd;
    
    float a = 0.168;
    float b = 0.2505;
    
    float FL_PWM_Target;
    float FR_PWM_Target;
    float BL_PWM_Target;
    float BR_PWM_Target;

    FL_PWM_Target = (vy_motor_input + (vx_motor_input/COS45) + a*w_motor_input + b*w_motor_input)/3;  
    FR_PWM_Target = (vy_motor_input - (vx_motor_input/COS45) - a*w_motor_input - b*w_motor_input)/3;
    BL_PWM_Target= (vy_motor_input - (vx_motor_input/COS45) + a*w_motor_input + b*w_motor_input)/3; 
    BR_PWM_Target = (vy_motor_input + (vx_motor_input/COS45) - a*w_motor_input - b*w_motor_input)/3; 
    if (abs(FL_PWM_Target - this->FL_pwm)> 0.05){
        this->FL_pwm += abs(FL_PWM_Target - this->FL_pwm)/(FL_PWM_Target - this->FL_pwm)*0.05;
    }
    if (abs(FR_PWM_Target - this->FR_pwm)> 0.05){
        this->FR_pwm += abs(FR_PWM_Target - this->FR_pwm)/(FR_PWM_Target - this->FR_pwm)*0.05;
    }
    if (abs(BL_PWM_Target - this->BL_pwm)> 0.05){
        this->BL_pwm += abs(BL_PWM_Target - this->BL_pwm)/(BL_PWM_Target - this->BL_pwm)*0.05;
    }
    if (abs(BR_PWM_Target - this->BR_pwm)> 0.05){
        this->BR_pwm += abs(BR_PWM_Target - this->BR_pwm)/(BR_PWM_Target - this->BR_pwm)*0.05;
    }
}

void ControlMecanum::getVars(float *vx_cmd, float *vy_cmd, float *w_cmd,
                        float *v_C_curr,
                        float *FL_pwm, float *FR_pwm, float *BR_pwm, float *BL_pwm, float *C_pwm,
                        float *C_target_speed) {
    *vx_cmd = this->vx_cmd;
    *vy_cmd = this->vy_cmd;
    *w_cmd = this->w_cmd;
    *v_C_curr = this->v_C_curr;
    *FL_pwm = this->FL_pwm;
    *FR_pwm = this->FR_pwm;
    *BR_pwm = this->BR_pwm;
    *BL_pwm = this->BL_pwm;
    *C_pwm = this->C_pwm;
    *C_target_speed = this->C_target_speed;
}