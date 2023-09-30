#ifndef __SENSORS_H
#define __SENSORS_H

#include "config.h"
#include "stdint.h"
#include "encoders.h"
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define NUM_SENSORS 4
#define UMBRAL_RUIDO_INFERIOR 140 // umbrales de ruido para dejar de leer ruido y poner 0 (calibrados!)
#define UMBRAL_RUIDO_SUPERIOR 2320

#define UMBRAL_DETECCION_FRONTAL 870 // supuestamete calibrado
#define UMBRAL_DETECCION_FRONTAL 870 // supuestamete calibrado

#define MAGNITUD_FILTRO 20

enum SENSORS { SENSOR_FRONT_LEFT = 0,
               SENSOR_LEFT = 1,
               SENSOR_RIGHT = 2,
               SENSOR_FRONT_RIGHT = 3};

uint8_t *get_sensors(void);
uint8_t get_sensors_num(void);
volatile uint16_t *get_sensors_raw(void);
uint16_t get_sensor_raw(enum SENSORS index);
uint16_t get_sensor_calibrated(enum SENSORS index);
uint16_t get_sensor_filtered(enum SENSORS index);
uint16_t get_sensor_mapped(enum SENSORS index);
bool get_sensor_bool(enum SENSORS index);
void actualizar_bool(int i_sensor);


// filtros, usar solo uno de estos dentro de sys_tick_handler() en main
void filtro_sensores(void);
void filtro_paso_bajo_1(void);
void filtro_paso_bajo_2(void);

#endif