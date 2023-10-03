#include "setup.h"
#include "usart.h"
#include "sensors.h"
#include "motors.h"
#include "controlHand.h"

#define ever ;;

bool set_fiesta_cambios = false;
bool configurado = false;
bool iniciado = false;

void sys_tick_handler(void) {
  clock_tick();
  update_encoder_readings();
  filtro_media();
}

int main(void) {
	setup();
	gpio_set(GPIOB, GPIO0);

	while(true){		
		if(iniciado == true) {
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

		}else{
			if(set_fiesta_cambios == false){
				if(get_sensor_bool(SENSOR_LEFT)){
					set_motors_speed(-30, -30);
					delay(500);
					set_motors_speed(0, 0);
					setFiesta(true);
					set_fiesta_cambios = true;
				}
				if(get_sensor_bool(SENSOR_RIGHT)){
					set_motors_speed(30, 30);
					delay(500);
					set_motors_speed(0, 0);
					set_fiesta_cambios = true;
				}
			}else{
				if(configurado == false){
					if(get_sensor_bool(SENSOR_LEFT)){
						set_motors_speed(30, 0);
						delay(500);
						set_motors_speed(0, 0);
						setMano(IZQUIERDA);
						configurado = true;
					}
					if(get_sensor_bool(SENSOR_RIGHT)){
						set_motors_speed(0, 30);
						delay(500);
						set_motors_speed(0, 0);
						setMano(DERECHA);
						configurado = true;
					}
				}else{
					for(ever){
						if(get_sensor_bool(SENSOR_FRONT_LEFT)){
							break;
						}
					}
					delay(2000);
					setObjetivos(get_sensor_mapped(SENSOR_LEFT), get_sensor_mapped(SENSOR_RIGHT));
					iniciado = true;
				}
			}
		}
	}
  return 0;
}