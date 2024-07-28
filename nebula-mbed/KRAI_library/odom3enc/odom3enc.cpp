/***************************************************************************
 * Title      : Library untuk odometri base
 * Name       : odom3enc.cpp
 * Version    : 2.0
 * Author     : Carlios Eryan TF 21
 * Date       : 7 Oktober 2022
 * Description:
 *
 * Library ini digunakan untuk melakukan perhitungan posisi robot menggunakan tiga encoder (dua untuk sumbu y/forward dan satu sumbu x/strafe)
 *
 *
 ***************************************************************************/

/******************************* library ***********************************/

#include "odom3enc.h"

// Constants
#ifndef PI
#define PI 3.14159265359
#define PPRX 2060 // 1000 // karena x4 jadi ppr * 4 = 1000
#define PPRY 2060 // 1000 // karena x4 jadi ppr * 4 = 1000
// #define p 2400
#endif

#ifndef D_RODA
#define D_RODA 5.8 // 0.06 (dalam cm)
#define R 51.662// R: jarak pusat ke encL&encR (dalam cm)
#define K 10.8// K: jarak pusat ke encAux (dalam cm)
#endif
/*************************** inisiasi class *******************************/
// odometriKRAI::odometriKRAI(encoderHAL *encX, encoderHAL *encY, CMPS12_KRAI *kompass)
odom3enc::odom3enc(encoderHAL *encL, encoderHAL *encR, encoderHAL *encAux)
{
    this->position.x = 0; // initiate all Value
    this->position.y = 0;
    this->position.teta = 0;
    this->encL = encL;
    this->encR = encR;
    this->encAux = encAux;
}

/*************************** definisi fungsi ******************************/
/* update position from base */
// void odometriKRAI::updatePosition(void)
void odom3enc::updatePosition(void)
{
    float l = (float) this->encL->getPulses(1) * PI * D_RODA / PPRY; /* butuh 1.5us */
    float r = (float) this->encR->getPulses(1) * PI * D_RODA / PPRY; /* butuh 1.5us */
    float aux = (float) this->encAux->getPulses(1) * PI * D_RODA / PPRX; /* butuh 1.5us */
    this->position.teta += (l-r)/(2*R);
    float fwdSpeed = (l+r)/2;

    float strSpeed = aux - K*(l-r)/(2*R);

    this->position.x += (fwdSpeed*sin(this->position.teta)+strSpeed*cos(this->position.teta)); // * cos(this->position.teta) - ((float)yTemp * PI * D_RODA / PPR) * -sin(this->position.teta); /* butuh 4.5 us */
    this->position.y += (fwdSpeed*cos(this->position.teta)-strSpeed*sin(this->position.teta)); // * sin(this->position.teta) - ((float)yTemp * PI * D_RODA / PPR) * cos(this->position.teta);  /* butuh 4.5 us */
    // printf("Pos  x: %f y: %f theta: %f\n", this->position.x, this->position.y, this->position.teta*180/PI);
    // printf("l: %f, r: %f, aux: %f Pos  x: %f y: %f theta: %f fwd: %f str: %f\n", l, r, aux, this->position.x, this->position.y, this->position.teta*180/PI, fwdSpeed, strSpeed);
}

void odom3enc::setPosition(float x_input, float y_input, float theta_input)
{
    this->position.x = x_input;
    this->position.y = y_input;
    this->position.teta = theta_input;
}

/* to reset all the position */
void odom3enc::resetOdom(void)
{
    this->position.x = 0; // initiate all Value
    this->position.y = 0;
    this->position.teta = 0;
}
