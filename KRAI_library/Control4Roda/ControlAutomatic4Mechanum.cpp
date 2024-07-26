#include "ControlAutomatic4Mechanum.h"

/* Constructor */
ControlAutomatic4Mechanum::ControlAutomatic4Mechanum(Motor *FL_motor, Motor *FR_motor, Motor *BR_motor, Motor *BL_motor, encoderKRAI *encFL, encoderKRAI *encFR, encoderKRAI *encBR, encoderKRAI *encBL, 
        ControlMotor *control_FL_motor, ControlMotor *control_FR_motor, ControlMotor *control_BR_motor, ControlMotor *control_BL_motor, odom2enc *odom, 
        pidLo *vxPid, pidLo *vyPid, pidLo *wPid, StanleyPursuit *line, pidLo *pid, pidLo *pid2): 
        Control4Mechanum(FL_motor, FR_motor, BR_motor, BL_motor, encFL, encFR, encBR, encBL, control_FL_motor, control_FR_motor, control_BR_motor, control_BL_motor, odom, vxPid, vyPid, wPid, line, pid, pid2){    
    this->otomatis = false;
}

/*Getter*/
bool ControlAutomatic4Mechanum::getOtomatis(){
    return this->otomatis;
}

bool ControlAutomatic4Mechanum::getParallelParkMode(){
    return this->parallel_park_mode;
}

float ControlAutomatic4Mechanum::getUltrasonic1(){
    return this->ultrasonic1;
}

float ControlAutomatic4Mechanum::getUltrasonic2(){
    return this->ultrasonic2;
}


/*Setter*/
void ControlAutomatic4Mechanum::setParallelParkMode(bool parallel_park_mode){
    this->parallel_park_mode = parallel_park_mode;
}

void ControlAutomatic4Mechanum::setOtomatis(bool otomatis) {
    this->otomatis = otomatis;
}

void ControlAutomatic4Mechanum::setUltrasonic1(float ultrasonic1){
    this->ultrasonic1 = ultrasonic1;
}

void ControlAutomatic4Mechanum::setUltrasonic2(float ultrasonic2){
    this->ultrasonic2 = ultrasonic2;
}


void ControlAutomatic4Mechanum::baseSpeed(){
    Control4Mechanum::baseSpeed();
    if (this->otomatis) {
        this->speed_base.x = (this->odom->position.x - this->last_pos.x)*(S_TO_US/SAMP_BASE_SPEED_US);
        this->last_pos.x = this->odom->position.x;
        
        this->speed_base.y = (this->odom->position.y - this->last_pos.y)*(S_TO_US/SAMP_BASE_SPEED_US);
        this->last_pos.y = this->odom->position.y;
        
        this->last_pos.teta = this->odom->position.teta;
    }
}

void ControlAutomatic4Mechanum::base() {
    // if (this->otomatis) { // Bakal kepake di nasional mungkin
    //     /* Odom 3 enc*/
    //     // this->line->updatePosition(this->odom->position.x, this->odom->position.y, this->odom->position.teta, &this->setpoint, &this->feedback, &this->max);
    //     // this->d_out = this->pid->createpwm(0, this->line->getError(), 0.5);
    //     // this->v = this->pid2->createpwm(0, -this->line->getT(), 0.5);
    //     // this->vx_motor = this->v * this->line->getVi() + this->d_out * this->line->getDi();
    //     // this->vy_motor = this->v * this->line->getVj() + this->d_out * this->line->getDj();
    //     // // this->w_motor = this->wPid->createpwm(this->setpoint, this->feedback, this->max);
    //     // this->w_motor = 0;
    //     // // this->line->setW((-1) * this->w_motor);

    //     // this->line->printVars();
    //     // // printf("vx = %f; vy = %f; w = %f", this->vx_motor, this->vy_motor, this->w_motor);
        
    //     // printf("x  = %f, y = %f, teta = %f; \n", this->odom->position.x, this->odom->position.y, this->odom->position.teta*180/3.1415);
    

    //     /* Odom2enc */
    //     // this->vx_motor = this->vxPid->createpwm(this->arr_x_test2[0], this->odom->position.x, 0.5);
        
    //     // move in y direction, set vx to 0
    //     if(this->initialPos.x == this->arr_x_test2[this->curr_dest_cout]){
    //         this->vx_motor = 0;
    //         this->vy_motor = this->vyPid->createpwm(this->arr_y_test2[this->curr_dest_cout], this->odom->position.y, this->max_pwm_velocity);
    //     }
    //     // move in x direction, set vy to 0
    //     else if(this->initialPos.y == this->arr_y_test2[this->curr_dest_cout]){
    //         this->vx_motor = this->vxPid->createpwm(this->arr_x_test2[this->curr_dest_cout], this->odom->position.x, this->max_pwm_velocity);
    //         this->vy_motor = 0;
    //     }
        
    //     printf("targetX: %f targetY: %f posX: %f posY: %f vx: %f vy: %f\n", this->arr_x_test2[this->curr_dest_cout], this->arr_y_test2[this->curr_dest_cout], this->odom->position.x, this->odom->position.y, this->vx_motor, this->vy_motor);

    
    // }
    // else if (this-> robot_to_pole_middle_mode){
    if(this->robot_to_pole_middle_mode){
        // Menengahkan robot ke pole
        if(!this->middle){
            this->w_motor = 0;
            this->vx_motor = 0;
            this->vy_motor = -0.25 * this->vyPid->createpwm(0, this->ultrasonic2 - this->ultrasonic1, 0.25);
            printf("(MIDDLIZATION) ultrasonic1 = %f\tultrasonic2 = %f\tvx_motor = %f\n", this->ultrasonic1, this->ultrasonic2, this->vx_motor);
            if(abs(this->ultrasonic2 - this->ultrasonic1) < 1){
                printf("SUDAH DI TENGAH\n");
                this->vx_motor = 0;
                this->vy_motor = 0;
                this->w_motor = 0;
                this->middle = true;
                this->deltaOffset = this->odom->position.x - (offset_for_lay_up - (this->ultrasonic1+this->ultrasonic2)/2) ;
            }
        }
        // Mengatur jarak robot dengan pole
        else if(this->offset_from_pole_mode){
            this->w_motor = 0;
            this->vx_motor = 0.25 * this->vxPid->createpwm(this->deltaOffset, this->odom->position.x , 0.25);
            printf("(OFFSETIZATION) Offset = %f\tDelta offset = %f\tvx_motor = %f\n", this->offset_for_lay_up,this->deltaOffset, this->vx_motor);
            this->vy_motor = 0;
            if(abs((this->ultrasonic1 + this->ultrasonic2)/2 - this->offset_for_lay_up) < 1){
                this->vx_motor = 0;
                this->vy_motor = 0;
                this->w_motor = 0;
                this->forceBrakeSync();
                printf("READY TO LAY UP\n");
                this->setRobotToPoleMiddleMode(false);
            }
        }
        // Diam
        else{
            this->vx_motor = 0;
            this->vy_motor = 0;
            this->w_motor = 0;
            // this->forceBrakeSync();

        }
    }

    else if(this->parallel_park_mode){
        this->vx_motor = 0;
        this->vy_motor = 0;
        this->w_motor = 1.5 * this->wPid->createpwm(0, this->ultrasonic1-this->ultrasonic2, 1);
        printf("w_motor = %f\n", this->w_motor);
    }
    
    else {
        // Robot jalannya lurus, ga perlu koreksi pake vc vy w PID
        this->vy_motor = this->vy_cmd;
        this->vx_motor = this->vx_cmd;
        this->w_motor = this->w_cmd;
    }
    
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

void ControlAutomatic4Mechanum::updatePosition(){
    Control4Roda::updatePosition();
    if (this->otomatis) {
        /* for odom 3 enc*/
        // if (this->line->TargetReached() == true && this->mode < 3) {
            
            
        //     if(this->curr_dest_cout == 2 ){
        //         printf("HAAAAALLLLLLLLLOOOOOOOOOOOO\n");
        //         this->forceBrakeSync();
        //     }
        //     else{
        //         printf("Ganti target woi shabri kntl\n");
        //         this->curr_dest_cout++;
        //         this->forceBrakeSync();
        //     }
            
            
        //     this->line->initialPosition(this->odom->position.x, this->odom->position.y, this->odom->position.teta);
        //     if (mode == 1) {
        //         printf("Masuk mode 1\n");
        //         this->line->setTarget((float)(this->arr_x_test[this->curr_dest_cout]), (float)(this->initialPos.y + this->arr_y_test[this->curr_dest_cout]));
        //     }
        //     else if (mode == 2) {
        //         printf("Masuk mode 2\n");
        //         this->line->setTarget((float)(this->arr_x_test[this->curr_dest_cout]), (float)(this->initialPos.y + this->arr_y_test[this->curr_dest_cout]));
        //     }
        // }

        /* for odom2enc */ 
        // move in y direction
        if(this->initialPos.x == this->arr_x_test2[this->curr_dest_cout]){
            // if target Y is reached
            if(abs((this->odom->position.y -this->arr_y_test2[this->curr_dest_cout])) < this->error){
                // set initial position into new coordinate
                this->initialPos.y = this->arr_y_test2[this->curr_dest_cout];
                this->curr_dest_cout++;
            }
        }
        // move in x direction
        else if(this->initialPos.y == this->arr_y_test2[this->curr_dest_cout]){
            // if target X is reached
            if(abs((this->odom->position.x - this->arr_x_test2[this->curr_dest_cout])) < this->error){
                // set initial position into new coordinate
                this->initialPos.x = this->arr_x_test2[this->curr_dest_cout];
                this->curr_dest_cout++;
            }
        }

        if(this->curr_dest_cout == this->total_destination){
            this->otomatis = false;
        }
    }    
}

bool ControlAutomatic4Mechanum::getRobotToPoleMiddleMode(){
    return this->robot_to_pole_middle_mode;
}
bool ControlAutomatic4Mechanum::getOffsetFromPoleMode(){
    return this->offset_from_pole_mode;
}

void ControlAutomatic4Mechanum::setRobotToPoleMiddleMode(bool robot_to_pole_middle_mode){
    this->robot_to_pole_middle_mode = robot_to_pole_middle_mode;
    this->middle = false;
    this->offset_from_pole_mode = false;
    this->deltaOffset = 0;
}

void ControlAutomatic4Mechanum::setOffsetFromPoleMode(bool offset_from_pole_mode){
    this->offset_from_pole_mode = offset_from_pole_mode;
}