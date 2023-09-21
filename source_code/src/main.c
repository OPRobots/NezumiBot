#include "setup.h"
#include "usart.h"
#include "sensors.h"
#include "encoders.h"
#include "motors.h"
#include "controlHand.h"

void sys_tick_handler(void) {
  clock_tick();
  update_encoder_readings();
  filtro_sensores();
}

int main(void) {
	setup();
	gpio_set(GPIOB, GPIO0);
	while (true) {
		/////
		
		//controlMano();

		/////
		//robotacion
		 
		for(int i = 0; i< get_sensors_num(); i++){
    		printf("S%d: %4d\t", i, get_sensor_raw(i));
    	}
		//printf("EI: %ld\t", get_encoder_left_total_ticks());
		//printf("ED: %ld\t", get_encoder_right_total_ticks());
		
		//set_motors_speed(0, 0);
		printf(" Sensores raw\n");
    	printf("S0: %4d\tS1: %4d\tS2: %4d\tS3: %4d\t", s0, s1, s2, s3);
		printf(" Sensores filtrados\n");

    	
    }
  return 0;
}