#include <motors.h>
#include <utils.h>

void set_motors_speed(float velI, float velD) {
  float ocI = 0;
  float ocD = 0;

  if (velI != 0) {
    ocI = map(abs(velI), 0, 100, 0, MOTORES_MAX_PWM);
    if (velI > 0) {
      gpio_set(GPIOA, GPIO6);
      gpio_clear(GPIOA, GPIO7);
    } else {
      gpio_set(GPIOA, GPIO7);
      gpio_clear(GPIOA, GPIO6);
    }
  }

  if (velD != 0) {
    ocD = map(abs(velD), 0, 100, 0, MOTORES_MAX_PWM);
    if (velD > 0) {
      gpio_set(GPIOB, GPIO1);
      gpio_clear(GPIOB, GPIO2);
    } else {
      gpio_set(GPIOB, GPIO2);
      gpio_clear(GPIOB, GPIO1);
    }
  }
  // printf("%ld - %ld\n", (uint32_t)ocI, (uint32_t)ocD);
  timer_set_oc_value(TIM2, TIM_OC1, (uint32_t)ocI);
  timer_set_oc_value(TIM2, TIM_OC3, (uint32_t)ocD);
}
