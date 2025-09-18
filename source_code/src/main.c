#include "setup.h"
#include "usart.h"
#include "sensors.h"
#include "motors.h"
#include "controlHand.h"

#define ever ;;

bool set_fiesta_cambios = false;
bool configurado = false;
bool iniciado = false;
bool new_data_ready = false;

void sys_tick_handler(void) {
  clock_tick();
  filtro_media(); //en desuso
  //adc_to_distance_loginv();
  //filtro_media_paso_bajo_1();   // filtrado y actualización
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
	//set_sensors_robot_calibration();

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

			//delay(80);
			//printf(">SF-Izq:%d\t", get_sensor_filtered(SENSOR_FRONT_LEFT));
			//printf(">S-Izq:%d\t", get_sensor_filtered(SENSOR_LEFT));
			//printf(">S-Der:%d\t", get_sensor_filtered(SENSOR_RIGHT));
			//printf(">SF-Der:%d\n", get_sensor_filtered(SENSOR_FRONT_RIGHT));
		
			//printf("Sensor %d: distancia = %u, Sensor %d: distancia = %u\n", SENSOR_FRONT_LEFT, sensors_distance[SENSOR_FRONT_LEFT], SENSOR_FRONT_RIGHT, sensors_distance[SENSOR_FRONT_RIGHT]);

			//test motores
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
						if(get_sensor_bool(SENSOR_FRONT_LEFT)||	gpio_get(GPIOB, GPIO9)){
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







//Codigo para recalibrar sensores con el script de caracterizacion
// /*
// 	#define WINDOW_SIZE 10
// 	uint32_t buffer[WINDOW_SIZE] = {0};
// 	uint8_t idx = 0;
// 	bool filled = false;


// uint32_t rolling_average(uint32_t new_val) {
//     buffer[idx] = new_val;
//     idx = (idx + 1) % WINDOW_SIZE;

//     if (idx == 0) filled = true;

//     uint64_t sum = 0;
//     uint8_t count = filled ? WINDOW_SIZE : idx;
//     for (uint8_t i = 0; i < count; i++) {
//         sum += buffer[i];
//     }
//     return (uint32_t)(sum / count);
// }
// */

// int main(void) {
// 	setup();
// 	gpio_set(GPIOB, GPIO0);
// 	set_sensors_robot_calibration();

	
// 	/*enum SENSORS { SENSOR_FRONT_LEFT = 0,
//                SENSOR_LEFT = 1,
//                SENSOR_RIGHT = 2,
//                SENSOR_FRONT_RIGHT = 3};*/
// 	uint8_t sensor = SENSOR_FRONT_RIGHT;

// 	/*uint32_t max_sensor = 0;
// 	uint32_t min_sensor = UINT32_MAX; // valor inicial máximo posible
// 	delay(50);
// 	sensor_buffer_init(sensor); // inicializa con la lectura actual del sensor
// 	*/

// 	while(true){		
// 		delay(60);

// 		/*
// 		uint32_t average = sensor_get_average_wait_new(sensor);

// 		// Actualizamos histórico
// 		if (average > max_sensor) max_sensor = average;
// 		if (average < min_sensor) min_sensor = average;
// 		uint32_t avg = rolling_average(average);
// 		// Imprimimos media, máximo y mínimo
// 		printf("Sensor %d: valor = %u, max = %u, min = %u, media = %u\n",
// 			sensor, average, max_sensor, min_sensor, avg);

// 		delay(200);
// 		*/
// 		printf("Sensor %d: distancia = %u, Sensor %d: distancia = %u\n",
// 			SENSOR_FRONT_LEFT, sensors_distance[SENSOR_FRONT_LEFT],sensor, sensors_distance[sensor]);
// 	}
//   return 0;
// }
