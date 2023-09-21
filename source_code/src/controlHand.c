#include "controlHand.h"

bool run = false; 
int objetivo_D;
int objetivo_I;
int error = 0;
int velBase = 500;

bool mano = false;

float p = 0;
float d = 0;
float kp = 0.03;
float ki = 0;
float kd = 0;
float kf = 0.8; // constante que determina cuanto afecta el sensor frontal para los giros dinamicos
int sumError = 0;
int ultError = 0;
int correccion = 0;
bool frontal = false;
float millis_PID = 0;
float micros_filtro = 0;

bool start = false;

//bool parpadeo_led_D = false;
//bool parpadeo_led_I = false;

bool debug = false;

unsigned long startedMillis = 0;

bool iniciado = false;

void controlMano(void){  
    //////////////////////////////////////////////
    ////         TODO EL CODIGO!!!!          /////
    //////////////////////////////////////////////


    //digitalWrite(LED_ADELANTE, LOW);
    //check_reference_wall_change(startedMillis, mano);

    if (mano == IZQUIERDA) {
    error = objetivo_I - get_sensor_raw(3);
        if (frontal && !DINAMICO) {
            set_motors_speed(0, 0);
            delay(70);
            set_motors_speed(0, -300);
            delay(200);
            set_motors_speed(0, 0);
            return;
        }
    }
    
    if (mano == DERECHA) {
    error = objetivo_D - get_sensor_raw(2);
        if (frontal && !DINAMICO) {
            set_motors_speed(0, 0);
            delay(70);
            set_motors_speed(0, 300);
            delay(200);
            set_motors_speed(0, 0);
            return;
        }
    }
    
    if (DINAMICO && frontal) { // Añadir lectura delantera al error para tener giros dinamicos
    if (mano == IZQUIERDA) {
        error -= get_sensor_raw(0) * kf;
    }
    
    if (mano == DERECHA) {
        error += get_sensor_raw(0) * kf;
    };
    
    }

    p = kp * error;
    d = kd * (error - ultError);
    ultError = error;
    correccion = p + d;
    if (mano == DERECHA) {
    correccion = -correccion;
    }
    set_motors_speed(velBase, correccion);

    //////////////////////////////////////////////
    ////     Y HASTA AQUI EL CODIGO!!!!      /////
    //////////////////////////////////////////////

}
                    