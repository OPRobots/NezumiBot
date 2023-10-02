#ifndef __CONTROLHAND_H
#define __CONTROLHAND_H


#include "motors.h"
#include "sensors.h"
//////////////////////

// Indica los millis a partir de los cuales realiza un cambio de pared de
// referencia (0 -> sin cambio)
//////////////////////

#define DERECHA 0
#define IZQUIERDA 1

#define TIEMPO_FILTRO 20
#define DINAMICO false
#define MAX_ERROR_PID_D 140 // si se sube este valor el arco de giro aumenta // 125 // 140
#define MAX_ERROR_PID_I 240 // si se sube este valor el arco de giro aumenta  160 // 135

void setObjetivos(int objetivoI, int objetivoD);
void controlMano(void);




#endif