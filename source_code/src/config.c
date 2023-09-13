#include "config.h"


float _micrometers_per_tick;

uint16_t get_start_millis(void) {
  return MILLIS_INICIO;
}

/**
 * @brief Establece la configuración de encoders
 * 
 */
static void set_micrometers_per_tick(void) {
  _micrometers_per_tick = MICROMETERS_PER_TICK_MICROMOUSE;
}

/**
 * @brief Obtiene la configuración de encoders
 * 
 * @return uint16_t MICROMETERS_PER_TICK_ROBOTRACER | MICROMETERS_PER_TICK_LINEFOLLOWER
 */
float get_micrometers_per_tick(void) {
  return _micrometers_per_tick;
}