/***************************************************************************
 * Title      : Library untuk odometri base
 * Name       : odometriKrai.cpp
 * Version    : 1.0
 * Author     : Oktavianus Irvan Sitanggang EL 18
 * Date       : 17 Desember 2019
 * Description:
 *
 * Library ini digunakan untuk melakukan perhitungan posisi robot
 *
 *
 ***************************************************************************/

/******************************* library ***********************************/

#include "odom2enc.h"

// Constants
#ifndef PI
#define PI 3.14159265359
#define PPRX 2000 // 4000 // karena x4 jadi ppr * 4 = 4000 //2053
#define PPRY 2000 // 4000 // karena x4 jadi ppr * 4 = 4000
#define p 2400
#endif

#ifndef D_RODA
#define D_RODA 5.8 // 0.06
#endif
/*************************** inisiasi class *******************************/
odom2enc::odom2enc(encoderHAL *encX, encoderHAL *encY)
{
    this->position.x = 0; // initiate all Value
    this->position.y = 0;
    this->position.teta = 0;
    this->encX = encX;
    this->encY = encY;
}

/*************************** definisi fungsi ******************************/
/* update position from base */
void odom2enc::updatePosition(void)
{
    int xTemp = this->encX->getPulses(1); /* butuh 1.5us */
    int yTemp = this->encY->getPulses(1); /* butuh 1.5us */

    

    this->position.x += -((float)xTemp * PI * D_RODA / PPRX); // * cos(this->position.teta) - ((float)yTemp * PI * D_RODA / PPR) * -sin(this->position.teta); /* butuh 4.5 us */
    this->position.y += ((float)yTemp * PI * D_RODA / PPRY); // * sin(this->position.teta) - ((float)yTemp * PI * D_RODA / PPR) * cos(this->position.teta);  /* butuh 4.5 us */
    // printf("Pos  x: %f y: %f\n", this->position.x, this->position.y);
    // printf("%d %d\n", xTemp, yTemp);
}

void odom2enc::setPosition(float x_input, float y_input)
{
    this->position.x = x_input;
    this->position.y = y_input;

    
    // printf("CompassValue set position : %f\n", this->kompass->compassValue());
}

/* to reset all the position */
void odom2enc::resetOdom(void)
{
    this->position.x = 0; // initiate all Value
    this->position.y = 0;
}