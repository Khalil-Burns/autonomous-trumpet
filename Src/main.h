#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#include "stm32_mmap.h"

#define DEMCR_P (*(volatile uint32_t*)0xE000EDFC)
#define DWT_CTRL_P (*(volatile uint32_t*)0xE0001000)
#define DWT_CYCCNT_P (*(volatile uint32_t*)0xE0001004)

#define SYSTEM_CORE_CLOCK 16000000

#define PERIPHERAL_BASE 0x40000000

#define APB1_BASE PERIPHERAL_BASE
#define TIM2_BASE (APB1_BASE + 0x0000)

#define TIM2_CR1_P   (*(volatile uint32_t*)(TIM2_BASE + 0x00))
#define TIM2_CCMR1_P (*(volatile uint32_t*)(TIM2_BASE + 0x18))
#define TIM2_CCER_P  (*(volatile uint32_t*)(TIM2_BASE + 0x20))
#define TIM2_PSC_P   (*(volatile uint32_t*)(TIM2_BASE + 0x28))
#define TIM2_ARR_P   (*(volatile uint32_t*)(TIM2_BASE + 0x2C))
#define TIM2_CCR1_P  (*(volatile uint32_t*)(TIM2_BASE + 0x34))

#define AHB1_OFFSET 0x20000
#define AHB1_BASE (PERIPHERAL_BASE + AHB1_OFFSET)

#define RCC_OFFSET 0x3800
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)
#define RCC_P ((volatile rcc_reg_t*)(RCC_BASE))

#define GPIOA_OFFSET 0x0
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)
#define GPIOA_P ((volatile gpio_reg_t*)(GPIOA_BASE))

void init_gpio_pin(uint32_t pin) {
	GPIOA_P->MODER &= ~(0b11 << (pin * 2));
	GPIOA_P->MODER |= 0b1 << (pin * 2);
}

void gpio_pin_high(uint32_t pin) {
	GPIOA_P->ODR |= (1 << pin);
}

void gpio_pin_low(uint32_t pin) {
	GPIOA_P->ODR &= ~(1 << pin);
}

void gpio_pin_toggle(uint32_t pin) {
	GPIOA_P->ODR ^= (1 << pin);
}

void init_timing(void) {
    // Enable the trace and debug block
    DEMCR_P |= (1 << 24); 
    // Reset the cycle counter
    DWT_CYCCNT_P = 0;     
    // Enable the cycle counter
    DWT_CTRL_P |= (1 << 0); 
}

void delay_s(uint32_t seconds) {
    uint32_t cycles = seconds * SYSTEM_CORE_CLOCK;
    uint32_t start = DWT_CYCCNT_P;
    
    while ((DWT_CYCCNT_P - start) < cycles);
}

void delay_ms(uint32_t ms) {
    uint32_t cycles = ms * (SYSTEM_CORE_CLOCK / 1000);
    uint32_t start = DWT_CYCCNT_P;
    
    while ((DWT_CYCCNT_P - start) < cycles);
}

void delay_us(uint32_t us) {
    uint32_t cycles = us * (SYSTEM_CORE_CLOCK / 1000000);
    uint32_t start = DWT_CYCCNT_P;
    
    while ((DWT_CYCCNT_P - start) < cycles);
}

#endif /* MAIN_H */
