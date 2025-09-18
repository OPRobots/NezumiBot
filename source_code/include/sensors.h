#ifndef __SENSORS_H
#define __SENSORS_H

#include "config.h"
#include "stdint.h"
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define NUM_SENSORS 4
#define UMBRAL_RUIDO_INFERIOR 140 //10 // umbrales de ruido para dejar de leer ruido y poner 0 (calibrados!)
#define UMBRAL_RUIDO_SUPERIOR 2320 //180

#define UMBRAL_DETECCION_FRONTAL 870 //100 // supuestamete calibrado 870

#define MAGNITUD_FILTRO 20

#define SENSOR_FRONT_CALIBRATION_READINGS 20
#define SENSOR_SIDE_CALIBRATION_READINGS 100

enum SENSORS { SENSOR_FRONT_LEFT = 0,
               SENSOR_LEFT = 1,
               SENSOR_RIGHT = 2,
               SENSOR_FRONT_RIGHT = 3};

struct sensors_distance_calibration {
  float a;
  float b;
  float c;
};

uint8_t *get_sensors(void);
uint8_t get_sensors_num(void);
volatile uint16_t *get_sensors_raw(void);
uint16_t get_sensor_raw(enum SENSORS index);
uint16_t get_sensor_calibrated(enum SENSORS index);
uint16_t get_sensor_filtered(enum SENSORS index);
uint16_t get_sensor_mapped(enum SENSORS index);
bool get_sensor_bool(enum SENSORS index);
void actualizar_bool(int i_sensor);

void side_sensors_calibration(void);
void front_sensors_calibration(void);

extern volatile uint16_t sensors_distance[NUM_SENSORS];

void adc_to_distance_loginv(void);

void sensor_buffer_init(uint8_t sensor);
uint32_t sensor_get_average_wait_new(uint8_t sensor);
uint16_t sensor_get_average(uint8_t sensor, uint16_t n_samples);


// filtros, usar solo uno de estos dentro de sys_tick_handler() en main
void filtro_media(void);
void filtro_media_paso_bajo_1(void);
void filtro_paso_bajo_1(void);
void filtro_paso_bajo_2(void);

#endif