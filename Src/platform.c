#include <stdint.h>
#include <stddef.h>
#include "main.h"

/* UART ------------------------------------------------------------------ */
void init_uart2(void) {
    /* Enable USART2 Clock */
    RCC_P->RCC_APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Configure PA2 (TX) and PA3 (RX) for AF7 */
    GPIOA_P->MODER &= ~((0x3U << (2U * 2U)) | (0x3U << (3U * 2U)));
    GPIOA_P->MODER |=  ((0x2U << (2U * 2U)) | (0x2U << (3U * 2U)));
    GPIOA_P->AFRL &= ~((0xFU << (2U * 4U)) | (0xFU << (3U * 4U)));
    GPIOA_P->AFRL |=  ((0x7U << (2U * 4U)) | (0x7U << (3U * 4U)));

    /* 16MHz / (16 * 115200) ~= 8.68 -> BRR ~ 0x0683 for 115200 */
    USART2_BRR_P = 0x0683U;

    /* Enable USART, RX and TX */
    USART2_CR1_P |= (1U << 13) | (1U << 2) | (1U << 3);
}

void init_uart1(void) {
    /* Enable USART1 Clock */
    RCC_P->RCC_APB2ENR |= RCC_APB2ENR_USART1EN;

    /* Configure PA9/PA10 AF7 */
    GPIOA_P->MODER &= ~((0x3U << (9U * 2U)) | (0x3U << (10U * 2U)));
    GPIOA_P->MODER |=  ((0x2U << (9U * 2U)) | (0x2U << (10U * 2U)));
    GPIOA_P->AFRH &= ~((0xFU << ((9U - 8U) * 4U)) | (0xFU << ((10U - 8U) * 4U)));
    GPIOA_P->AFRH |=  ((0x7U << ((9U - 8U) * 4U)) | (0x7U << ((10U - 8U) * 4U)));

    USART1_BRR_P = 0x003EU; /* example */
    USART1_CR1_P |= (1U << 13) | (1U << 2) | (1U << 3);
}

char uart_read_char(void) {
    while (!(USART2_SR_P & (1U << 5)));
    return (char)(USART2_DR_P & 0xFFU);
}

uint8_t uart_read_byte(void) {
    while (!(USART2_SR_P & (1U << 5)));
    return (uint8_t)(USART2_DR_P & 0xFFU);
}

void uart_write_char(char c) {
    while (!(USART2_SR_P & (1U << 7)));
    USART2_DR_P = (uint32_t)(c & 0xFF);
}

void uart_write_string(const char* str) {
    while (str && *str) {
        uart_write_char(*str++);
    }
}

void uart_write_hex(uint8_t val) {
    const char hex[] = "0123456789ABCDEF";
    uart_write_char(hex[(val >> 4) & 0xF]);
    uart_write_char(hex[val & 0xF]);
}

void uart_write_int(int val) {
    char buf[16];
    int i = 0;
    unsigned int v;

    if (val == 0) { uart_write_char('0'); return; }
    if (val < 0) { uart_write_char('-'); v = (unsigned int)(-val); }
    else { v = (unsigned int)val; }

    while (v > 0 && i < (int)sizeof(buf)-1) {
        buf[i++] = (char)('0' + (v % 10U));
        v /= 10U;
    }
    while (i--) uart_write_char(buf[i]);
}

/* GPIO helpers ---------------------------------------------------------- */
void init_gpio_pin(gpio_reg_t* gpio, uint32_t pin) {
    gpio->MODER &= ~(0x3U << (pin * 2U));
    gpio->MODER |=  (0x1U << (pin * 2U));
}

void gpio_pin_high(gpio_reg_t* gpio, uint32_t pin) { gpio->ODR |= (1U << pin); }
void gpio_pin_low(gpio_reg_t* gpio, uint32_t pin)  { gpio->ODR &= ~(1U << pin); }
void gpio_pin_toggle(gpio_reg_t* gpio, uint32_t pin){ gpio->ODR ^= (1U << pin); }

void init_gpioa_pin(uint32_t pin) { init_gpio_pin(GPIOA_P, pin); }
void gpioa_pin_high(uint32_t pin) { gpio_pin_high(GPIOA_P, pin); }
void gpioa_pin_low(uint32_t pin)  { gpio_pin_low(GPIOA_P, pin); }
void gpioa_pin_toggle(uint32_t pin){ gpio_pin_toggle(GPIOA_P, pin); }

void init_gpiob_pin(uint32_t pin) { init_gpio_pin(GPIOB_P, pin); }
void gpiob_pin_high(uint32_t pin) { gpio_pin_high(GPIOB_P, pin); }
void gpiob_pin_low(uint32_t pin)  { gpio_pin_low(GPIOB_P, pin); }
void gpiob_pin_toggle(uint32_t pin){ gpio_pin_toggle(GPIOB_P, pin); }

/* Timing --------------------------------------------------------------- */
void init_timing(void) {
    DEMCR_P |= (1U << 24);
    DWT_CYCCNT_P = 0U;
    DWT_CTRL_P |= (1U << 0);
}

uint32_t get_ticks_us(void) {
    return DWT_CYCCNT_P / (SYSTEM_CORE_CLOCK / 1000000U);
}

void delay_s(uint32_t seconds) {
    uint32_t cycles = seconds * SYSTEM_CORE_CLOCK;
    uint32_t start = DWT_CYCCNT_P;
    while ((DWT_CYCCNT_P - start) < cycles);
}

void delay_ms(uint32_t ms) {
    uint32_t cycles = ms * (SYSTEM_CORE_CLOCK / 1000U);
    uint32_t start = DWT_CYCCNT_P;
    while ((DWT_CYCCNT_P - start) < cycles);
}

void delay_us(uint32_t us) {
    uint32_t cycles = us * (SYSTEM_CORE_CLOCK / 1000000U);
    uint32_t start = DWT_CYCCNT_P;
    while ((DWT_CYCCNT_P - start) < cycles);
}

/* TIM2 PWM helpers ----------------------------------------------------- */
static void init_tim2_pwm_ch1(void) {
    TIM2_CCMR1_P &= ~((0x7U << 4U) | (1U << 3U));
    TIM2_CCMR1_P |= (0x6U << 4U) | (1U << 3U);
    TIM2_CCER_P |= (1U << 0U);
    TIM2_CCR1_P = 0U;
}

static void init_tim2_pwm_ch2(void) {
    TIM2_CCMR1_P &= ~((0x7U << 12U) | (1U << 11U));
    TIM2_CCMR1_P |= (0x6U << 12U) | (1U << 11U);
    TIM2_CCER_P |= (1U << 4U);
    TIM2_CCR2_P = 0U;
}

static void init_tim2_pwm_ch3(void) {
    TIM2_CCMR2_P &= ~((0x7U << 4U) | (1U << 3U));
    TIM2_CCMR2_P |= (0x6U << 4U) | (1U << 3U);
    TIM2_CCER_P |= (1U << 8U);
    TIM2_CCR3_P = 0U;
}

void init_tim2_pwm(void) {
    RCC_P->RCC_APB1ENR |= (1U << 0U);
    TIM2_PSC_P = 16U - 1U;
    TIM2_ARR_P = 1000U - 1U;
    init_tim2_pwm_ch1();
    init_tim2_pwm_ch2();
    init_tim2_pwm_ch3();
    TIM2_CR1_P |= (1U << 0U);
}

void init_tim2_pwm_gpioa_pin(int pin) {
    GPIOA_P->MODER &= ~(0x3U << (pin * 2U));
    GPIOA_P->MODER |=  (0x2U << (pin * 2U));
    if (pin < 8) {
        GPIOA_P->AFRL &= ~(0xFU << (pin * 4U));
        GPIOA_P->AFRL |=  (0x1U << (pin * 4U));
    } else {
        GPIOA_P->AFRH &= ~(0xFU << ((pin - 8) * 4U));
        GPIOA_P->AFRH |=  (0x1U << ((pin - 8) * 4U));
    }
}

void init_tim2_pwm_gpiob_pin(int pin) {
    GPIOB_P->MODER &= ~(0x3U << (pin * 2U));
    GPIOB_P->MODER |=  (0x2U << (pin * 2U));
    if (pin < 8) {
        GPIOB_P->AFRL &= ~(0xFU << (pin * 4U));
        GPIOB_P->AFRL |=  (0x1U << (pin * 4U));
    } else {
        GPIOB_P->AFRH &= ~(0xFU << ((pin - 8) * 4U));
        GPIOB_P->AFRH |=  (0x1U << ((pin - 8) * 4U));
    }
}

void set_motor1_speed(uint16_t value) { if (value > 1000U) value = 1000U; TIM2_CCR1_P = value; }
void set_motor2_speed(uint16_t value) { if (value > 1000U) value = 1000U; TIM2_CCR2_P = value; }
void set_motor3_speed(uint16_t value) { if (value > 1000U) value = 1000U; TIM2_CCR3_P = value; }

/* Valve control -------------------------------------------------------- */
void set_valve(int valve_number, int state) {
    if ((unsigned)valve_number >= VALVES) return;
    int out1_pin = valve_pins[valve_number][0];
    int out2_pin = valve_pins[valve_number][1];

    if (state == 1) {
        gpioa_pin_high(out1_pin);
        gpioa_pin_low(out2_pin);
    } else if (state == -1) {
        gpioa_pin_low(out1_pin);
        gpioa_pin_high(out2_pin);
    } else {
        gpioa_pin_low(out1_pin);
        gpioa_pin_low(out2_pin);
    }

    valve_states[valve_number] = state;
}

void play_note(const char* note) {
    if (!note) return;
    for (int i = 0; i < VALVES; ++i) {
        int state = note[i] - '0';
        if (state == 0 && valve_states[i] == 1) {
            set_valve(i, -1);
        }
    }
    delay_ms(10);
    for (int i = 0; i < VALVES; ++i) {
        int state = note[i] - '0';
        if (state == valve_states[i]) continue;
        set_valve(i, state);
    }
}
