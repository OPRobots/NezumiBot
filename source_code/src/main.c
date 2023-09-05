#include "setup.h"
#include "usart.h"
#include "sensors.h"
#include "encoders.h"

void sys_tick_handler(void) {
  clock_tick();
  update_encoder_readings();
}

int main(void) {
	setup();
	while (true) {
		//robotacion
		printf("Funciona?\n");
		/* for(int i = 0; i< get_sensors_num(); i++){
    		printf("S%d: %d\t", i, get_sensor_raw(i));
    	} */
		printf("EI: %ld\t", get_encoder_left_total_ticks());
		printf("ED: %ld\t", get_encoder_right_total_ticks());
    }
  return 0;
}