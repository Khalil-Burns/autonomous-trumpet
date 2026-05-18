#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include "stm32_mmap.h"
#include "trumpet_notes.h"

/* Cortex DWT registers for microsecond timing */
#define DEMCR_P (*(volatile uint32_t*)0xE000EDFC)
#define DWT_CTRL_P (*(volatile uint32_t*)0xE0001000)
#define DWT_CYCCNT_P (*(volatile uint32_t*)0xE0001004)

#define SYSTEM_CORE_CLOCK 16000000U

#define PERIPHERAL_BASE 0x40000000U
#define APB1_BASE PERIPHERAL_BASE

#define TIM2_BASE       (APB1_BASE + 0x0000U)
#define TIM2_CR1_P      (*(volatile uint32_t*)(TIM2_BASE + 0x00U))
#define TIM2_CCMR1_P    (*(volatile uint32_t*)(TIM2_BASE + 0x18U))
#define TIM2_CCMR2_P    (*(volatile uint32_t*)(TIM2_BASE + 0x1CU))
#define TIM2_CCER_P     (*(volatile uint32_t*)(TIM2_BASE + 0x20U))
#define TIM2_PSC_P      (*(volatile uint32_t*)(TIM2_BASE + 0x28U))
#define TIM2_ARR_P      (*(volatile uint32_t*)(TIM2_BASE + 0x2CU))
#define TIM2_CCR1_P     (*(volatile uint32_t*)(TIM2_BASE + 0x34U))
#define TIM2_CCR2_P     (*(volatile uint32_t*)(TIM2_BASE + 0x38U))
#define TIM2_CCR3_P     (*(volatile uint32_t*)(TIM2_BASE + 0x3CU))
#define TIM2_CCR4_P     (*(volatile uint32_t*)(TIM2_BASE + 0x40U))

#define AHB1_OFFSET 0x20000U
#define AHB1_BASE (PERIPHERAL_BASE + AHB1_OFFSET)

#define RCC_OFFSET 0x3800U
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)
#define RCC_P ((volatile rcc_reg_t*)(RCC_BASE))

#define GPIOA_OFFSET 0x0U
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)
#define GPIOA_P ((volatile gpio_reg_t*)(GPIOA_BASE))

#define GPIOB_OFFSET 0x400U
#define GPIOB_BASE (AHB1_BASE + GPIOB_OFFSET)
#define GPIOB_P ((volatile gpio_reg_t*)(GPIOB_BASE))

#define USART2_BASE     (0x40004400U)
#define USART2_SR_P     (*(volatile uint32_t*)(USART2_BASE + 0x00U))
#define USART2_DR_P     (*(volatile uint32_t*)(USART2_BASE + 0x04U))
#define USART2_BRR_P    (*(volatile uint32_t*)(USART2_BASE + 0x08U))
#define USART2_CR1_P    (*(volatile uint32_t*)(USART2_BASE + 0x0CU))

#define USART1_BASE     (0x40011000U)
#define USART1_SR_P     (*(volatile uint32_t*)(USART1_BASE + 0x00U))
#define USART1_DR_P     (*(volatile uint32_t*)(USART1_BASE + 0x04U))
#define USART1_BRR_P    (*(volatile uint32_t*)(USART1_BASE + 0x08U))
#define USART1_CR1_P    (*(volatile uint32_t*)(USART1_BASE + 0x0CU))

#define RCC_APB2ENR_USART1EN (1U << 4)
#define RCC_APB1ENR_USART2EN (1U << 17)

/* Pin aliases */
#define PA1 1U
#define PA4 4U
#define PA5 5U
#define PA6 6U
#define PA7 7U
#define PA8 8U
#define PA9 9U
#define PA10 10U
#define PB10 10U

#define VALVES 3
#define VALVE_1_OUT1 PA6
#define VALVE_1_OUT2 PA7
#define VALVE_2_OUT1 PA8
#define VALVE_2_OUT2 PA9
#define VALVE_3_OUT1 PA4
#define VALVE_3_OUT2 PA10

/* Global valve state storage (defined in main.c) */
extern int valve_states[VALVES];
extern int valve_pins[VALVES][2];

/* Public API - prototypes */
void init_uart2(void);
void init_uart1(void);
char uart_read_char(void);
uint8_t uart_read_byte(void);
void uart_write_char(char c);
void uart_write_string(const char* str);
void uart_write_hex(uint8_t val);
void uart_write_int(int val);

void init_gpio_pin(gpio_reg_t* gpio, uint32_t pin);
void gpio_pin_high(gpio_reg_t* gpio, uint32_t pin);
void gpio_pin_low(gpio_reg_t* gpio, uint32_t pin);
void gpio_pin_toggle(gpio_reg_t* gpio, uint32_t pin);
void init_gpioa_pin(uint32_t pin);
void gpioa_pin_high(uint32_t pin);
void gpioa_pin_low(uint32_t pin);
void gpioa_pin_toggle(uint32_t pin);
void init_gpiob_pin(uint32_t pin);
void gpiob_pin_high(uint32_t pin);
void gpiob_pin_low(uint32_t pin);
void gpiob_pin_toggle(uint32_t pin);

void init_timing(void);
uint32_t get_ticks_us(void);
void delay_s(uint32_t seconds);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

void init_tim2_pwm(void);
void init_tim2_pwm_gpioa_pin(int pin);
void init_tim2_pwm_gpiob_pin(int pin);
void set_motor1_speed(uint16_t value);
void set_motor2_speed(uint16_t value);
void set_motor3_speed(uint16_t value);

void set_valve(int valve_number, int state);
void play_note(const char* note);

#endif /* MAIN_H */
