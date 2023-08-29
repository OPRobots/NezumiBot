#include "sensors.h"

static uint8_t sensors_adc[NUM_SENSORS] = {ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3};
static volatile uint16_t sensors_raw[NUM_SENSORS];

static bool sensors_digital[NUM_SENSORS];
static uint8_t sensors_weight[NUM_SENSORS];
static uint8_t sensors_position = 0;

uint8_t get_sensors_num() {
  return NUM_SENSORS;
}

uint8_t *get_sensors() {
  return sensors_adc;
}

volatile uint16_t *get_sensors_raw() {
  return sensors_raw;
}

uint16_t get_sensor_raw(enum SENSORS index) {
  return sensors_raw[index];
}

uint16_t get_sensor_calibrated(enum SENSORS index) {
    if (sensors_raw[index] > RIVAL_SENSOR_THRESHOLD && sensors_raw[index] < RIVAL_SENSOR_MAX) {
        return sensors_raw[index];
    } else {
        return 0;
    }
}

bool get_sensor_digital(enum SENSORS index) {
    if (sensors_raw[index] > RIVAL_SENSOR_THRESHOLD && sensors_raw[index] < RIVAL_SENSOR_MAX) {
        return true;
    }
    return false;
}

void update_sensors_readings(void) {
  uint32_t sensor_avg = 0;
  uint32_t sensor_sum = 0;
  for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++) {
    sensors_digital[sensor] = sensors_raw[sensor] > RIVAL_SENSOR_THRESHOLD;
    sensors_weight[sensor] = sensors_raw[sensor] > RIVAL_SENSOR_THRESHOLD ? map(sensors_raw[sensor], RIVAL_SENSOR_THRESHOLD, RIVAL_SENSOR_MAX, 1, 10) : 0;

    if (sensors_digital[sensor]) {
      sensor_avg += sensors_weight[sensor] * (sensor * 1) * 1000;
      sensor_sum += sensors_weight[sensor];
    }
  }
  sensors_position = sensor_avg / sensor_sum - (NUM_SENSORS) * 1000 / 2;
}