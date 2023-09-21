#include "sensors.h"

static uint8_t sensors_adc[NUM_SENSORS] = {ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3};
static volatile uint16_t sensors_raw[NUM_SENSORS];

static bool sensors_digital[NUM_SENSORS];
static uint8_t sensors_weight[NUM_SENSORS];
static uint8_t sensors_position = 0;


//////////////////// filtro
const int UMBRAL = 1700;

bool s0_bool = false;
bool s1_bool = false;
bool s2_bool = false;
bool s3_bool = false;

int s0 = 0;
int s1 = 0;
int s2 = 0;
int s3 = 0;

int Filtro_s0[MAGNITUD_FILTRO];
int Filtro_s1[MAGNITUD_FILTRO];
int Filtro_s2[MAGNITUD_FILTRO];
int Filtro_s3[MAGNITUD_FILTRO];

int i_s = 0;
////////////////////

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

void filtro_sensores(void) {

  Filtro_s0[i_s] = get_sensor_raw(0);
  Filtro_s1[i_s] = get_sensor_raw(1);
  Filtro_s2[i_s] = get_sensor_raw(2);
  Filtro_s3[i_s] = get_sensor_raw(3);
  i_s = (i_s + 1) % MAGNITUD_FILTRO; // Avanza el índice circularmente cuando supera MAGNITUD FILTRO vuelve a ser 0

  s0 = 0;
  s1 = 0;
  s2 = 0;
  s3 = 0;
  
  for (int i = 0; i < MAGNITUD_FILTRO; i++) {
    s0 += Filtro_s1[i];
    s1 += Filtro_s1[i];
    s2 += Filtro_s2[i];
    s3 += Filtro_s3[i];
  }

  s0 = s0 / MAGNITUD_FILTRO;
  s1 = s1 / MAGNITUD_FILTRO;
  s2 = s2 / MAGNITUD_FILTRO;
  s3 = s3 / MAGNITUD_FILTRO;

  s0_bool = s0 > UMBRAL;
  s1_bool = s1 > UMBRAL;
  s2_bool = s2 > UMBRAL;
  s3_bool = s3 > UMBRAL;
}