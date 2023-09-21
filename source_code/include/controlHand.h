#ifndef __CONTROLHAND_H
#define __CONTROLHAND_H


#include "motors.h"
//////////////////////

// Indica los millis a partir de los cuales realiza un cambio de pared de
// referencia (0 -> sin cambio)
//////////////////////

#define DERECHA 0
#define IZQUIERDA 1

#define DETECCION_FRONTAL 180
#define TIEMPO_FILTRO 20
#define DINAMICO false


void controlMano(void);




#endif