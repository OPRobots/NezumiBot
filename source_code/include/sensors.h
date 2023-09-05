#ifndef __SENSORS_H
#define __SENSORS_H

#include "config.h"
#include "stdint.h"
#include "encoders.h"
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define NUM_SENSORS 4
#define RIVAL_SENSOR_THRESHOLD 550
#define RIVAL_SENSOR_MAX 2600
#define LINE_SENSOR_THRESHOLD 2000

enum SENSORS { SENSOR_FRONT_RIGHT = 0,
               SENSOR_FRONT_LEFT = 1,
               SENSOR_LEFT = 2,
               SENSOR_RIGHT = 3};

uint8_t *get_sensors(void);
uint8_t get_sensors_num(void);
volatile uint16_t *get_sensors_raw(void);
uint16_t get_sensor_raw(enum SENSORS index);
uint16_t get_sensor_calibrated(enum SENSORS index);
bool get_sensor_digital(enum SENSORS index);

void update_sensors_readings(void);

#endif