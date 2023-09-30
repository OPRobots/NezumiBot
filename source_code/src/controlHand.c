#include "controlHand.h"

int objetivo_D = 0;
int objetivo_I = 0;
int error = 0;
int velBase = 30;

bool mano = DERECHA;

float p = 0;
float d = 0;

float kp = 0.1;
float kd = 0;
float kf = 0;  // constante que determina cuanto afecta el sensor frontal para los giros dinamicos

int ultError = 0;

int correccion = 0;
bool frontal = false;

// bool parpadeo_led_D = false;
// bool parpadeo_led_I = false;

unsigned long startedMillis = 0;

bool iniciado = false;

void controlMano(void) {
    //////////////////////////////////////////////
    ////         TODO EL CODIGO!!!!          /////
    //////////////////////////////////////////////

    if (get_sensor_bool(SENSOR_FRONT_LEFT)) {
        frontal = true;
    } else {
        frontal = false;
    }

    if (mano == DERECHA) {
        if (get_sensor_mapped(SENSOR_RIGHT) > 0) {
            error = objetivo_D - get_sensor_mapped(SENSOR_RIGHT);
        } else {
            error = MAX_ERROR_PID;
        }

        if (frontal && !DINAMICO) {
            //set_motors_speed(0, 0);
            //delay(50);
            //set_motors_speed(40, -40);
            //delay(100);
            //set_motors_speed(0, 0);
            //return;
        }
    }

    // Serial.println(error);
    // if (MAX_ERROR_PID != 0) {
    //   error = constrain(error, -MAX_ERROR_PID, MAX_ERROR_PID);
    // }

    if (DINAMICO && frontal) {  // Añadir lectura delantera al error para tener giros dinamicos
        error -= get_sensor_mapped(SENSOR_FRONT_LEFT) * kf;
    }

    p = kp * error;
    d = kd * (error - ultError);
    ultError = error;
    correccion = p + d;
    if (mano == DERECHA) {
        correccion = -correccion;
    }
    
    
    //printf("correccio: %5d\n", correccion);
    set_motors_speed(velBase - correccion, velBase + correccion);
    
    //////////////////////////////////////////////
    ////     Y HASTA AQUI EL CODIGO!!!!      /////
    //////////////////////////////////////////////

}

