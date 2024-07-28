#include "Control4Roda.h"

/*Constructor*/
Control4Roda::Control4Roda(Motor *FL_motor, Motor *FR_motor, Motor *BR_motor, Motor *BL_motor, encoderKRAI *encFL, encoderKRAI *encFR, encoderKRAI *encBR, encoderKRAI *encBL, ControlMotor *control_FL_motor, ControlMotor *control_FR_motor, ControlMotor *control_BR_motor, ControlMotor *control_BL_motor, odom2enc *odom, pidLo *vxPid, pidLo *vyPid, pidLo *wPid, StanleyPursuit *line, pidLo *pid, pidLo *pid2) {
    this->FL_motor = FL_motor;
    this->FR_motor = FR_motor;
    this->BR_motor = BR_motor;
    this->BL_motor = BL_motor;
    this->encFL = encFL;
    this->encFR = encFR;
    this->encBR = encBR;
    this->encBL = encBL;
    this->control_FL_motor = control_FL_motor;
    this->control_FR_motor = control_FR_motor;
    this->control_BR_motor = control_BR_motor;
    this->control_BL_motor = control_BL_motor;
    this->odom = odom;
    this->vxPid = vxPid;
    this->vyPid = vyPid;
    this->wPid = wPid;
    this->pid = pid;
    this->pid2 = pid2;
    this->curr_dest_cout = 0;
    this->mode = 1;
    this->line = line;
    this->line->initialPosition(this->odom->position.x, this->odom->position.y, this->odom->position.teta);
    this->line->setTarget((float)(this->odom->position.x + arr_x_test[0]), (float)(this->odom->position.y + arr_y_test[0]));
    this->initialPos.x = this->odom->position.x;
    this->initialPos.y = this->odom->position.y;
}

/*Getter*/
void Control4Roda::getVars(float *vx_cmd, float *vy_cmd, float *w_cmd, 
                        float *v_FL_curr, float *v_FR_curr, float *v_BR_curr, float *v_BL_curr, 
                        float *FL_pwm, float *FR_pwm, float *BR_pwm, float *BL_pwm, 
                        float *FL_target_speed, float *FR_target_speed, float *BR_target_speed, float *BL_target_speed, 
                        float *vy_last, float *vx_last, float *w_last, 
                        float *vy_motor, float *vx_motor, float *w_motor,
                        float *x_pos, float *y_pos, float *theta_pos,
                        float *x_dest, float *y_dest) {
    *vx_cmd = this->vx_cmd;
    *vy_cmd = this->vy_cmd;
    *w_cmd = this->w_cmd;
    *v_FL_curr = this->v_FL_curr;
    *v_FR_curr = this->v_FR_curr;
    *v_BR_curr = this->v_BR_curr;
    *v_BL_curr = this->v_BL_curr;
    *FL_pwm = this->FL_pwm;
    *FR_pwm = this->FR_pwm;
    *BR_pwm = this->BR_pwm;
    *BL_pwm = this->BL_pwm;
    *FL_target_speed = this->FL_target_speed;
    *FR_target_speed = this->FR_target_speed;
    *BR_target_speed = this->BR_target_speed;
    *BL_target_speed = this->BL_target_speed;
    *vy_last = this->vy_last;
    *vx_last = this->vx_last;
    *w_last = this->w_last;
    *vy_motor = this->vy_motor;
    *vx_motor = this->vx_motor;
    *w_motor = this->w_motor;
    *x_pos = this->odom->position.x;
    *y_pos = this->odom->position.y;
    *theta_pos = this->odom->position.teta;
    if (mode == 1) {
        *x_dest = this->arr_x_test[this->curr_dest_cout];
        *y_dest = this->arr_y_test[this->curr_dest_cout];
    }
    else if (mode == 2) {
        *x_dest = this->arr_x_test[this->curr_dest_cout];
        *y_dest = this->arr_y_test[this->curr_dest_cout];
    }
}

/*Setter*/
void Control4Roda::set_vx_cmd(float vx_cmd) {
    this->vx_cmd = vx_cmd;
}

void Control4Roda::set_vy_cmd(float vy_cmd) {
    this->vy_cmd = vy_cmd;
}

void Control4Roda::set_w_cmd(float w_cmd) {
    this->w_cmd = w_cmd;
}



/*Procedure*/
void Control4Roda::motorSamp() {
    this->FL_motor->speed(this->FL_pwm);
    this->FR_motor->speed(this->FR_pwm);
    this->BR_motor->speed(this->BR_pwm); 
    this->BL_motor->speed(this->BL_pwm);
}

void Control4Roda::forceBrakeSync(){
    this->FL_motor->forcebrake();
    this->FR_motor->forcebrake();
    this->BR_motor->forcebrake();
    this->BL_motor->forcebrake();
}


void Control4Roda::pidMotorSamp(){
    /* menghitung pid motor base */     
    this->FL_pwm = this->control_FL_motor->createpwm(this->FL_target_speed, this->v_FL_curr, max_pwm);
    this->FR_pwm = this->control_FR_motor->createpwm(this->FR_target_speed, this->v_FR_curr, max_pwm);
    this->BR_pwm = this->control_BR_motor->createpwm(this->BR_target_speed, this->v_BR_curr, max_pwm);
    this->BL_pwm = this->control_BL_motor->createpwm(this->BL_target_speed, this->v_BL_curr, max_pwm);            
}

void Control4Roda::updatePosition(){
    this->odom->updatePosition();
    this->line->updatePosition(this->odom->position.x, this->odom->position.y, this->odom->position.teta, &this->setpoint, &this->feedback, &this->max);
}

