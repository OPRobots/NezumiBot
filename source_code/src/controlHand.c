#include "controlHand.h"

int objetivo_D = 0;
int objetivo_I = 0;
int error = 0;
int velBase = 40;

bool mano = IZQUIERDA;

float p = 0;
float d = 0;

float kp = 0.07; // estaba a 0.10 // 0.11       //0.07 aceptable
float kd = 0.22;    // estaba a //0.22 //-18


float kf = 0;    // constante que determina cuanto afecta el sensor frontal para los giros dinamicos

int ultError = 0;

int correccion = 0;
bool frontal = false;

unsigned long millis_PID = 1;
unsigned long time_millis = 0;

unsigned long millis_empieza_cambios = 45000;
unsigned long millis_cambio_mano = 30000;
unsigned long time_cambio_mano = 0;

bool toca_fiesta = false;
bool empezo_la_fiesta = false;

// bool parpadeo_led_D = false;
// bool parpadeo_led_I = false;

unsigned long startedMillis = 0;

void setObjetivos(int objetivoI, int objetivoD){
    objetivo_I = objetivoI;
    objetivo_D = objetivoD;
}

void setFiesta(bool fiesta){
    toca_fiesta = fiesta;
}

void setMano(bool mano_in){
    mano = mano_in;
}

void controlMano(void) {

    if(toca_fiesta == true){
        if (empezo_la_fiesta == false && (get_clock_ticks() - time_cambio_mano > millis_empieza_cambios)){
            mano = !mano;
            empezo_la_fiesta = true;
            time_cambio_mano = get_clock_ticks();
        } else if(empezo_la_fiesta == true && get_clock_ticks() - time_cambio_mano > millis_cambio_mano){
            mano = !mano;
            time_cambio_mano = get_clock_ticks();
        }
    }

    //////////////////////////////////////////////
    ////         TODO EL CODIGO!!!!          /////
    //////////////////////////////////////////////
    if ( get_clock_ticks() - time_millis >= millis_PID) { //ajustar

        if (get_sensor_bool(SENSOR_FRONT_LEFT)) {
            frontal = true;
        } else {
            frontal = false;
        }

        if (mano == IZQUIERDA) {
            if (get_sensor_bool(SENSOR_LEFT)) {
                error = objetivo_I - get_sensor_mapped(SENSOR_LEFT);
            } else {
                error = MAX_ERROR_PID_I;
            }

            if (frontal && !DINAMICO) {
                set_motors_speed(0, 0);
                delay(50);
                set_motors_speed(40, -40);
                delay(170);
                set_motors_speed(0, 0);
                delay(50); //100 //50
                return;
            }
        }

        if (mano == DERECHA) {
            if (get_sensor_bool(SENSOR_RIGHT)) {
                error = objetivo_D - get_sensor_mapped(SENSOR_RIGHT);
            } else {
                error = MAX_ERROR_PID_D;
            }

            if (frontal && !DINAMICO) {
                set_motors_speed(0, 0);
                delay(50);
                set_motors_speed(-40, 40);
                delay(170);
                set_motors_speed(0, 0);
                delay(50); //100 //50
                return;
            }
        }

        // Serial.println(error);
        // if (MAX_ERROR_PID != 0) {
        //   error = constrain(error, -MAX_ERROR_PID, MAX_ERROR_PID);
        // }

        if (DINAMICO && frontal) {  // Añadir lectura delantera al error para tener giros dinamicos
            error -= (get_sensor_mapped(SENSOR_FRONT_LEFT) - UMBRAL_DETECCION_FRONTAL) * kf;
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
        time_millis = get_clock_ticks();
    }
    //////////////////////////////////////////////
    ////     Y HASTA AQUI EL CODIGO!!!!      /////
    //////////////////////////////////////////////
    
}

