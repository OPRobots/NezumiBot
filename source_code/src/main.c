#include "setup.h"
#include "usart.h"
#include "sensors.h"
#include "encoders.h"
#include "motors.h"
#include "controlHand.h"

void sys_tick_handler(void) {
  clock_tick();
  update_encoder_readings();
  filtro_paso_bajo_1();
}

int main(void) {
	setup();
	gpio_set(GPIOB, GPIO0);
	delay(1000);
	setObjetivoD(get_sensor_mapped(SENSOR_RIGHT));
	while (true) {
		/////
		
		controlMano();


		/////
		//tests
		 
	






		 //prints
		//printf(">SF-Izq:%d\n", get_sensor_raw(SENSOR_FRONT_LEFT));
		//printf(">S-Izq:%d\n", get_sensor_raw(SENSOR_LEFT));
		//printf(">S-Der:%d\n", get_sensor_raw(SENSOR_RIGHT));
		//printf(">SF-Der:%d\n", get_sensor_raw(SENSOR_FRONT_RIGHT));

		//printf("SF-Izq: %4d\tS-Izq: %4d\tSF-Der: %4d\tSF-Der: %4d\n", get_sensor_raw(SENSOR_FRONT_LEFT), get_sensor_raw(SENSOR_LEFT), get_sensor_raw(SENSOR_RIGHT), get_sensor_raw(SENSOR_FRONT_RIGHT));

		//delay(50);
		//printf(">SF-Izq:%d\n", get_sensor_filtered(SENSOR_FRONT_LEFT));
		//printf(">S-Izq:%d\n", get_sensor_filtered(SENSOR_LEFT));
		//printf(">S-Der:%d\n", get_sensor_filtered(SENSOR_RIGHT));
		//printf(">SF-Der:%d\n", get_sensor_filtered(SENSOR_FRONT_RIGHT));

		//printf(">Encoder Izq: %ld\n", get_encoder_left_total_ticks());
		//printf(">Encoder Der: %ld\n", get_encoder_right_total_ticks());
	
		
		//set_motors_speed(-30, -30);
    	
    }
  return 0;
}