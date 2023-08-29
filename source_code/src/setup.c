#include "setup.h"

/**
 * @brief Configura los relojes principales del robot
 *
 * SYSCLK a 84 MHz
 * GPIO: A, B, C
 * USART3
 * Timers: 1-PWM, 4-ADDR_LED
 *
 */
static void setup_clock(void) {
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);

  rcc_periph_clock_enable(RCC_USART1);

  dwt_enable_cycle_counter();
}

/**
 * @brief Configura el SysTick para 1ms
 *
 */
static void setup_systick(void) {
  systick_set_frequency(SYSTICK_FREQUENCY_HZ, SYSCLK_FREQUENCY_HZ);
  systick_counter_enable();
  systick_interrupt_enable();
}

static void setup_timer_priorities(void) {
  nvic_set_priority(NVIC_SYSTICK_IRQ, 16 * 1);
  nvic_set_priority(NVIC_USART1_IRQ, 16 * 2);

  nvic_enable_irq(NVIC_USART1_IRQ);
}

static void setup_gpio(void) {
  // Builtin LED
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO13);

  // Botón de inicio //TODOO
  //gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO15);

  // Botones de menú //TODOO
  //gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO13 | GPIO14 | GPIO15);

  // Salida PWM para los motores
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO10 );
  gpio_set_af(GPIOB, GPIO_AF2, GPIO5 | GPIO10);

  // Entradas analógicas sensores
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3 );

  // USART1
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);
}

static void setup_usart(void) {
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  // USART_CR1(USART1) |= USART_CR1_RXNEIE;
  // usart_enable_tx_interrupt(USART1);
  usart_enable(USART1);
}



void setup(void) {
  setup_clock();
  setup_gpio();
  setup_usart();

  setup_timer_priorities();
  setup_systick();
}