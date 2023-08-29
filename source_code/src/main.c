#include "usart.h"
#include "sensors.h"

int main(void) {
	setup();
	while (true) {
		//robotacion
		printf("Hola?\n");
		for(int i = 0; i< get_sensors_num(); i++){
    		printf("%d: %d\t", i, get_sensor_raw(i));
    	}
    }
  return 0;
}