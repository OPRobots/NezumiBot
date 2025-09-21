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
  //update_encoder_readings();
  filtro_media();
}

/*
encender:

--menu fiesta de cambios--
sensor derecha: no hay fiesta
sensor izquierda: si hay fiesta

--menu seleccion mano--
sensor derecha: mano derecha
sensor izquierda: mano izquierda

--espera para iniciar--
sensor frontal izquierda: iniciar!
*/

int main(void) {
	setup();
	gpio_set(GPIOB, GPIO0);
	while(true){		
		//printf("SF-Izq: %4d\tS-Izq: %4d\tS-Der: %4d\tSF-Der: %4d\n", get_sensor_bool(SENSOR_FRONT_LEFT), get_sensor_bool(SENSOR_LEFT), get_sensor_bool(SENSOR_RIGHT), get_sensor_bool(SENSOR_FRONT_RIGHT));
		//printf("SF-Izq: %4d\tS-Izq: %4d\tS-Der: %4d\tSF-Der: %4d\n", get_sensor_filtered(SENSOR_FRONT_LEFT), get_sensor_filtered(SENSOR_LEFT), get_sensor_filtered(SENSOR_RIGHT), get_sensor_filtered(SENSOR_FRONT_RIGHT));

		if(iniciado == true) {
			
			printf("> En control iniciado\t");
			/////
			//controlMano();

			/////
			//tests

			//prints

			//delay(50);

			//printf(">SF-Izq:%d\n", get_sensor_raw(SENSOR_FRONT_LEFT));
			//printf(">S-Izq:%d\n", get_sensor_raw(SENSOR_LEFT));
			//printf(">S-Der:%d\n", get_sensor_raw(SENSOR_RIGHT));
			//printf(">SF-Der:%d\n", get_sensor_raw(SENSOR_FRONT_RIGHT));
			//printf("SF-Izq: %4d\tS-Izq: %4d\tSF-Der: %4d\tSF-Der: %4d\n", get_sensor_raw(SENSOR_FRONT_LEFT), get_sensor_raw(SENSOR_LEFT), get_sensor_raw(SENSOR_RIGHT), get_sensor_raw(SENSOR_FRONT_RIGHT));

			//printf(">SF-Izq:%d\n", get_sensor_filtered(SENSOR_FRONT_LEFT));
			//printf(">S-Izq:%d\n", get_sensor_filtered(SENSOR_LEFT));
			//printf(">S-Der:%d\n", get_sensor_filtered(SENSOR_RIGHT));
			//printf(">SF-Der:%d\n", get_sensor_filtered(SENSOR_FRONT_RIGHT));
			//printf("SF-Izq: %4d\tS-Izq: %4d\tS-Der: %4d\tSF-Der: %4d\n", get_sensor_filtered(SENSOR_FRONT_LEFT), get_sensor_filtered(SENSOR_LEFT), get_sensor_filtered(SENSOR_RIGHT), get_sensor_filtered(SENSOR_FRONT_RIGHT));

			//printf(">SF-Izq:%d\n", get_sensor_bool(SENSOR_FRONT_LEFT));
			//printf(">S-Izq:%d\n", get_sensor_bool(SENSOR_LEFT));
			//printf(">S-Der:%d\n", get_sensor_bool(SENSOR_RIGHT));
			//printf(">SF-Der:%d\n", get_sensor_bool(SENSOR_FRONT_RIGHT));
			//printf("SF-Izq: %4d\tS-Izq: %4d\tS-Der: %4d\tSF-Der: %4d\n", get_sensor_bool(SENSOR_FRONT_LEFT), get_sensor_bool(SENSOR_LEFT), get_sensor_bool(SENSOR_RIGHT), get_sensor_bool(SENSOR_FRONT_RIGHT));


			//printf(">Encoder Izq: %ld\n", get_encoder_left_total_ticks());
			//printf(">Encoder Der: %ld\n", get_encoder_right_total_ticks());

			
			//set_motors_speed(0, 40);


			
			set_motors_speed(30, 30);
			delay(150);
			set_motors_speed(0, 0);
			delay(100);
			set_motors_speed(-30, -30);
			delay(150);
			set_motors_speed(0, 0);
			delay(1000);
			set_motors_speed(30, 0);
			delay(150);
			set_motors_speed(0, 0);
			delay(100);
			set_motors_speed(-30, 0);
			delay(150);
			set_motors_speed(0, 0);
			delay(1000);
			set_motors_speed(0, 30);
			delay(150);
			set_motors_speed(0, 0);
			delay(100);
			set_motors_speed(0, -30);
			delay(150);
			set_motors_speed(0, 0);
			delay(1000);
			

		}else{
			if(set_fiesta_cambios == false){
				printf("> En fiesta de cambios\t");
				if(get_sensor_bool(SENSOR_LEFT) && get_sensor_bool(SENSOR_FRONT_LEFT)){
					set_motors_speed(-30, -30);
					delay(500);
					set_motors_speed(0, 0);
					setFiesta(true);
					set_fiesta_cambios = true;
				}
				if(get_sensor_bool(SENSOR_RIGHT) && get_sensor_bool(SENSOR_FRONT_RIGHT)){
					set_motors_speed(30, 30);
					delay(500);
					set_motors_speed(0, 0);
					set_fiesta_cambios = true;
				}
			}else{
				if(configurado == false){
					printf("> En configuracion\t");
					if(get_sensor_bool(SENSOR_LEFT) && get_sensor_bool(SENSOR_FRONT_LEFT)){
						set_motors_speed(30, 0);
						delay(500);
						set_motors_speed(0, 0);
						setMano(IZQUIERDA);
						configurado = true;
					}
					if(get_sensor_bool(SENSOR_RIGHT) && get_sensor_bool(SENSOR_FRONT_RIGHT)){
						set_motors_speed(0, 30);
						delay(500);
						set_motors_speed(0, 0);
						setMano(DERECHA);
						configurado = true;
					}
				}else{
					for(ever){
						printf("> En espera de inicio\n");
						if((get_sensor_bool(SENSOR_RIGHT) && get_sensor_bool(SENSOR_FRONT_RIGHT)) ||	gpio_get(GPIOB, GPIO9)){
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