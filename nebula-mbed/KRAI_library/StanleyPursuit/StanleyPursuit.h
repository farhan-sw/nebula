#ifndef __Stanley_H__
#define __Stanley_H__

#include <math.h>

class StanleyPursuit {
    private:
        float m;
        float X0;
        float Y0;
        float Xf;
        float Yf;
        float err1;
        float err2;
        float d;
        float theta;
        float di;
        float dj;
        float vi;
        float vj;
        float error;
        float t;
        float W;
        bool alreadySet;
        bool enterCondition;
        float sinTheta;
        float cosTheta;

    public:
        StanleyPursuit();
        // d : jarak perpendicullar posisi robot dengan garis acuan. Diperlukan pid untuk mengecilkan variabel ini
        // theta : sudut antara garis acuan dengan sumbu x
        // di,dj : pengali untuk d pada arah x dan y
        // vi,vj : pengali untuk v pada arah x dan y
        // error : toleransi yang diberikan dalam penentuan tercapainya target. Default 0
        // t : jarak posisi robot dengan titik tujuan. Paralel dengan garis

        void setError(float error);

        float getError();

        float getVi();
        float getVj();
        float getDi();
        float getDj();

        void initialPosition(float,float,float);
        //Set titik awal

        void setTarget(float,float);
        //Set titik akhir

        void updatePosition(float x, float y, float curr_theta, float *setpoint, float *feedback, float *max);
        //Update posisi sekaligus generate nilai d, theta, dan t

        void updateTheta(float theta_feed, float *setpoint, float *feedback, float *max);

        void setW(float W);
        // Set W as (-1)*w_cmd from PID

        bool TargetReached();

        void printVars();
        //Mengecek apakah titik akhir sudah dicapai
        float getT();
};
#endif // __Stanley_H__