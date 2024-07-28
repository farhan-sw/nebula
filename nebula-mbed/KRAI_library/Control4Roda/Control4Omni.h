#ifndef CONTROL4OMNI_H
#define CONTROL4OMNI_H

#include "Control4Roda.h"
#include "../../Master_Elephant/Configurations/Constants.h"
class Control4Omni : public Control4Roda {
    public:
        Control4Omni(Motor *FL_motor, Motor *FR_motor, Motor *BR_motor, Motor *BL_motor, encoderKRAI *encFL, encoderKRAI *encFR, encoderKRAI *encBR, encoderKRAI *encBL, 
        ControlMotor *control_FL_motor, ControlMotor *control_FR_motor, ControlMotor *control_BR_motor, ControlMotor *control_BL_motor, odom2enc *odom, 
        pidLo *vxPid, pidLo *vyPid, pidLo *wPid, StanleyPursuit *line, pidLo *pid, pidLo *pid2);

        void encoderMotorSamp();
        virtual void baseSpeed();
        virtual void base();
};

#endif 
