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

  rcc_periph_clock_enable(RCC_ADC1);
  rcc_periph_clock_enable(RCC_DMA2);
  
  rcc_periph_clock_enable(RCC_TIM2);

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
  nvic_set_priority(NVIC_DMA2_STREAM0_IRQ, 16 * 2);
  nvic_set_priority(NVIC_USART1_IRQ, 16 * 2);


  nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);
  nvic_enable_irq(NVIC_USART1_IRQ);
}

static void setup_gpio(void) {
  // Builtin LED
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO13);

  // Botón de inicio //TODOO
  //gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO15);

  // Botones de menú //TODOO
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO9);

  // Entradas analógicas sensores
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3 );

    // Salida PWM Motores
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5);
  gpio_set_af(GPIOA, GPIO_AF1, GPIO5);
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10);
  gpio_set_af(GPIOB, GPIO_AF1, GPIO10);

  // Salidas Digitales Motores
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6 | GPIO7 ); //6-bin2 7-bin1
  gpio_clear(GPIOA, GPIO6 | GPIO7);
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2); //posible 0 no necesario 0-stby 1-ain1 2ain2
  gpio_clear(GPIOB, GPIO0 | GPIO1 | GPIO2);

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

static void setup_adc1(void) {
  adc_power_off(ADC1);
  adc_disable_external_trigger_regular(ADC1);
  adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);
  adc_set_right_aligned(ADC1);
  // adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_15CYC);
  adc_enable_scan_mode(ADC1);

  adc_set_regular_sequence(ADC1, get_sensors_num(), get_sensors());
  adc_set_continuous_conversion_mode(ADC1);

  adc_power_on(ADC1);
  int i;
  for (i = 0; i < 800000; i++) {
    /* Wait a bit. */
    __asm__("nop");
  }

  adc_start_conversion_regular(ADC1);
}

static void setup_dma_adc1(void) {
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_DMA2EN);
  dma_stream_reset(DMA2, DMA_STREAM0);

  dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)&ADC_DR(ADC1));
  dma_set_memory_address(DMA2, DMA_STREAM0, (uint32_t)get_sensors_raw());
  dma_enable_memory_increment_mode(DMA2, DMA_STREAM0);
  dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_16BIT);
  dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_16BIT);
  dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_LOW);

  dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM0);
  // dma_enable_half_transfer_interrupt(DMA2, DMA_STREAM0);
  dma_set_number_of_data(DMA2, DMA_STREAM0, get_sensors_num());
  dma_enable_circular_mode(DMA2, DMA_STREAM0);
  dma_set_transfer_mode(DMA2, DMA_STREAM0, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);
  dma_channel_select(DMA2, DMA_STREAM0, DMA_SxCR_CHSEL_0);

  dma_enable_stream(DMA2, DMA_STREAM0);
  adc_enable_dma(ADC1);
  adc_set_dma_continue(ADC1);
}

/**
 * @brief Configura el TIM2 para manejar el PWM de los Motores, inicialmente a 4kHz
 * 
 */
static void setup_motors_pwm(void) {
  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  //84000000
  timer_set_prescaler(TIM2, rcc_apb2_frequency * 2 / 4000000 - 2);
  // 4000000 es la frecuencia a la que irá el PWM 4 kHz, los dos últimos ceros no se porqué, pero son necesarios ya que rcc_apb2_frequency también añade dos ceros a mayores
  timer_set_repetition_counter(TIM2, 0);
  timer_continuous_mode(TIM2);
  timer_set_period(TIM2, MOTORES_MAX_PWM);

  timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
  timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
  timer_set_oc_value(TIM2, TIM_OC1, 0);
  timer_set_oc_value(TIM2, TIM_OC3, 0);
  timer_enable_oc_output(TIM2, TIM_OC1);
  timer_enable_oc_output(TIM2, TIM_OC3);

  timer_enable_break_main_output(TIM2);

  timer_enable_counter(TIM2);
}

void dma2_stream0_isr(void) {
  if (dma_get_interrupt_flag(DMA2, DMA_STREAM0, DMA_TCIF)) {
    dma_clear_interrupt_flags(DMA2, DMA_STREAM0, DMA_TCIF);
  }
}

void setup(void) {
  setup_clock();
  setup_gpio();
  setup_usart();
  setup_timer_priorities();

  setup_dma_adc1();
  setup_adc1();
  setup_motors_pwm();

  setup_systick();
}