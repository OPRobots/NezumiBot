#include "sensors.h"

static uint8_t sensors_adc[NUM_SENSORS] = {ADC_CHANNEL3, ADC_CHANNEL2, ADC_CHANNEL1, ADC_CHANNEL0};
static volatile uint16_t sensors_raw[NUM_SENSORS];

//////////////////// filtro

bool sensor_bool[NUM_SENSORS] = {false};

int sensor_filtrado[NUM_SENSORS] = {0};

int sensor_sumado_filtro[NUM_SENSORS] = {0};

int filtro[NUM_SENSORS][MAGNITUD_FILTRO] = {0};

int i_filtro = 0;
//////////////////// filtro paso bajo


int sensor_filtrado_paso_bajo[3][NUM_SENSORS] = {{750}, {750}, {750}};
int sensor_raw[3][NUM_SENSORS] = {{750},{750},{750}};

float b[3] = {0.00024132, 0.00048264, 0.00024132};
float a[2] = {1.95558189, -0.95654717};


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
  if(index == SENSOR_FRONT_RIGHT){
    return sensors_raw[index] + 74;
  }
  if(index == SENSOR_LEFT){
    return sensors_raw[index] + 50;
  }
  return sensors_raw[index];
}

uint16_t get_sensor_calibrated(enum SENSORS index) { // devuelve 0 en las zonas que el sensor tiene ruido (cerca - lejos)  
    int valor_sensor = get_sensor_raw(index);
    if (valor_sensor > UMBRAL_RUIDO_INFERIOR && valor_sensor < UMBRAL_RUIDO_SUPERIOR) {
        return valor_sensor;
    } else if(valor_sensor >= UMBRAL_RUIDO_SUPERIOR) {
        return UMBRAL_RUIDO_SUPERIOR + 10;
    } else {
        return UMBRAL_RUIDO_INFERIOR - 10;
    }
}

uint16_t get_sensor_filtered(enum SENSORS index) { // filtrado con media de magnitud_filtro
  return sensor_filtrado[index];
}

uint16_t get_sensor_mapped(enum SENSORS index) { // filtrado con media de magnitud_filtro
  return map(sensor_filtrado[index], 140, 2320, 0, 1024);
}
bool get_sensor_bool(enum SENSORS index) {
    return sensor_bool[index];
}

void actualizar_bool(int i_sensor){
  if(i_sensor == 0 || i_sensor == 3){
    sensor_filtrado[0] = (sensor_filtrado[0] + sensor_filtrado[3])/2;
    sensor_bool[0] = sensor_filtrado[0] > UMBRAL_DETECCION_FRONTAL;
  }else if(i_sensor == 1 ){
    sensor_bool[i_sensor] = sensor_filtrado[i_sensor] > UMBRAL_DETECCION_FRONTAL;
  } else{
    sensor_bool[i_sensor] = sensor_filtrado[i_sensor] > UMBRAL_DETECCION_FRONTAL;
  }
}

void filtro_media(void) {

  for (int i_sensor = 0; i_sensor < NUM_SENSORS; i_sensor++){
    sensor_sumado_filtro[i_sensor] = sensor_sumado_filtro[i_sensor] - filtro[i_sensor][i_filtro]; // tenemos un sumatorio total, se resta el valor antiguo, se actualiza y se suma el nuevo.
    filtro[i_sensor][i_filtro] = get_sensor_calibrated(i_sensor); //actualizar valor
    sensor_sumado_filtro[i_sensor] = sensor_sumado_filtro[i_sensor] + filtro[i_sensor][i_filtro];
    sensor_filtrado[i_sensor] = sensor_sumado_filtro[i_sensor] / MAGNITUD_FILTRO;

    if(i_sensor == 0 || i_sensor == 3){
      sensor_filtrado[0] = (sensor_filtrado[0] + sensor_filtrado[3])/2;
    }
    actualizar_bool(i_sensor);
  }
  
  i_filtro = (i_filtro + 1) % MAGNITUD_FILTRO; // Avanza el índice circularmente cuando supera MAGNITUD FILTRO vuelve a ser 0
}

void filtro_media_paso_bajo_1(void){
  
  for (int i_sensor = 0; i_sensor < NUM_SENSORS; i_sensor++){
    sensor_raw[0][i_sensor] = get_sensor_calibrated(i_sensor);
    sensor_filtrado_paso_bajo[0][i_sensor] = 0.969*sensor_filtrado_paso_bajo[1][i_sensor] + 0.0155*sensor_raw[0][i_sensor] + 0.0155*sensor_raw[1][i_sensor]; // magnitud 5
    //sensor_filtrado_paso_bajo[0][i_sensor] = 0.993*sensor_filtrado_paso_bajo[1][i_sensor] + 0.00313*sensor_raw[0][i_sensor] + 0.00313*sensor_raw[1][i_sensor]; // magintud 2
    sensor_filtrado_paso_bajo[1][i_sensor] = sensor_filtrado_paso_bajo[0][i_sensor]; //guardamos valores anteriores
    sensor_raw[1][i_sensor] = sensor_raw[0][i_sensor];

    sensor_sumado_filtro[i_sensor] = sensor_sumado_filtro[i_sensor] - filtro[i_sensor][i_filtro]; // tenemos un sumatorio total, se resta el valor antiguo, se actualiza y se suma el nuevo.
    filtro[i_sensor][i_filtro] = sensor_filtrado_paso_bajo[0][i_sensor]; //actualizar valor
    sensor_sumado_filtro[i_sensor] = sensor_sumado_filtro[i_sensor] + filtro[i_sensor][i_filtro];
    sensor_filtrado[i_sensor] = sensor_sumado_filtro[i_sensor] / MAGNITUD_FILTRO;

    if(i_sensor == 0 || i_sensor == 3){
      sensor_filtrado[0] = (sensor_filtrado[0] + sensor_filtrado[3])/2;
    }
    actualizar_bool(i_sensor);
  }
  
  i_filtro = (i_filtro + 1) % MAGNITUD_FILTRO; // Avanza el índice circularmente cuando supera MAGNITUD FILTRO vuelve a ser 0
}

void filtro_paso_bajo_1(void){ // filtro paso bajo magnitud 1
  for (int i_sensor = 0; i_sensor < NUM_SENSORS; i_sensor++){
    sensor_raw[0][i_sensor] = get_sensor_calibrated(i_sensor);
    sensor_filtrado_paso_bajo[0][i_sensor] = 0.969*sensor_filtrado_paso_bajo[1][i_sensor] + 0.0155*sensor_raw[0][i_sensor] + 0.0155*sensor_raw[1][i_sensor]; // magnitud 5
    //sensor_filtrado_paso_bajo[0][i_sensor] = 0.993*sensor_filtrado_paso_bajo[1][i_sensor] + 0.00313*sensor_raw[0][i_sensor] + 0.00313*sensor_raw[1][i_sensor]; // magintud 2

    sensor_filtrado_paso_bajo[1][i_sensor] = sensor_filtrado_paso_bajo[0][i_sensor]; //guardamos valores anteriores
    sensor_raw[1][i_sensor] = sensor_raw[0][i_sensor];

    sensor_filtrado[i_sensor] = sensor_filtrado_paso_bajo[0][i_sensor]; // guardamos en la misma variables que los otros filtros para que sea intercambiable

    if(i_sensor == 0 || i_sensor == 3){
      sensor_filtrado[0] = (sensor_filtrado[0] + sensor_filtrado[3])/2;
    }
    actualizar_bool(i_sensor);
  }
}

void filtro_paso_bajo_2(void){ // filtro paso bajo magnitud 2 (falta ajustar la magnitud y recalcular los valores ya que filtra demasiado y tiende a 0)
  for (int i_sensor = 0; i_sensor < NUM_SENSORS; i_sensor++){
    sensor_raw[0][i_sensor] = get_sensor_calibrated(i_sensor);
    sensor_filtrado_paso_bajo[0][i_sensor] = a[0]*sensor_filtrado_paso_bajo[1][i_sensor] + a[1]*sensor_filtrado_paso_bajo[2][i_sensor] + b[0]*sensor_raw[0][i_sensor] + b[1]*sensor_raw[1][i_sensor] + b[2]*sensor_raw[2][i_sensor];
  
    sensor_filtrado_paso_bajo[2][i_sensor] = sensor_filtrado_paso_bajo[1][i_sensor]; // guardamos los anteriores
    sensor_filtrado_paso_bajo[1][i_sensor] = sensor_filtrado_paso_bajo[0][i_sensor];
    sensor_raw[2][i_sensor] = sensor_raw[1][i_sensor]; 
    sensor_raw[1][i_sensor] = sensor_raw[0][i_sensor]; 

    sensor_filtrado[i_sensor] = sensor_filtrado_paso_bajo[0][i_sensor]; // guardamos en la misma variables que los otros filtros para que sea intercambiable
    actualizar_bool(i_sensor);
  }
}