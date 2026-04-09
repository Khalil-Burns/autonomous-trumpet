#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#include "stm32_mmap.h"
#include "trumpet_notes.h"

#define DEMCR_P (*(volatile uint32_t*)0xE000EDFC)
#define DWT_CTRL_P (*(volatile uint32_t*)0xE0001000)
#define DWT_CYCCNT_P (*(volatile uint32_t*)0xE0001004)

#define SYSTEM_CORE_CLOCK 16000000

#define PERIPHERAL_BASE 0x40000000

#define APB1_BASE PERIPHERAL_BASE

// #define TIM2_BASE (APB1_BASE + 0x0000)
// #define TIM2_CR1_P   (*(volatile uint32_t*)(TIM2_BASE + 0x00))
// #define TIM2_CCMR1_P (*(volatile uint32_t*)(TIM2_BASE + 0x18))
// #define TIM2_CCER_P  (*(volatile uint32_t*)(TIM2_BASE + 0x20))
// #define TIM2_PSC_P   (*(volatile uint32_t*)(TIM2_BASE + 0x28))
// #define TIM2_ARR_P   (*(volatile uint32_t*)(TIM2_BASE + 0x2C))
// #define TIM2_CCR1_P  (*(volatile uint32_t*)(TIM2_BASE + 0x34))

#define TIM2_BASE       (APB1_BASE + 0x0000)
#define TIM2_CR1_P      (*(volatile uint32_t*)(TIM2_BASE + 0x00))

// Mode Registers
#define TIM2_CCMR1_P    (*(volatile uint32_t*)(TIM2_BASE + 0x18)) // Controls CH1 & CH2
#define TIM2_CCMR2_P    (*(volatile uint32_t*)(TIM2_BASE + 0x1C)) // Controls CH3 & CH4

// Enable/Config Register
#define TIM2_CCER_P     (*(volatile uint32_t*)(TIM2_BASE + 0x20))

// Timing Registers
#define TIM2_PSC_P      (*(volatile uint32_t*)(TIM2_BASE + 0x28))
#define TIM2_ARR_P      (*(volatile uint32_t*)(TIM2_BASE + 0x2C))

// Duty Cycle Registers (Capture/Compare)
#define TIM2_CCR1_P     (*(volatile uint32_t*)(TIM2_BASE + 0x34)) // PA5 (usually)
#define TIM2_CCR2_P     (*(volatile uint32_t*)(TIM2_BASE + 0x38)) // PA1
#define TIM2_CCR3_P     (*(volatile uint32_t*)(TIM2_BASE + 0x3C)) // PA2
#define TIM2_CCR4_P     (*(volatile uint32_t*)(TIM2_BASE + 0x40)) // PA3

#define AHB1_OFFSET 0x20000
#define AHB1_BASE (PERIPHERAL_BASE + AHB1_OFFSET)

#define RCC_OFFSET 0x3800
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)
#define RCC_P ((volatile rcc_reg_t*)(RCC_BASE))

#define GPIOA_OFFSET 0x0
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)
#define GPIOA_P ((volatile gpio_reg_t*)(GPIOA_BASE))

#define GPIOB_OFFSET 0x400
#define GPIOB_BASE (AHB1_BASE + GPIOB_OFFSET)
#define GPIOB_P ((volatile gpio_reg_t*)(GPIOB_BASE))

#define USART2_BASE     (0x40004400)
#define USART2_SR_P     (*(volatile uint32_t*)(USART2_BASE + 0x00))
#define USART2_DR_P     (*(volatile uint32_t*)(USART2_BASE + 0x04))
#define USART2_BRR_P    (*(volatile uint32_t*)(USART2_BASE + 0x08))
#define USART2_CR1_P    (*(volatile uint32_t*)(USART2_BASE + 0x0C))

#define USART1_BASE     (0x40011000)
#define USART1_SR_P     (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_DR_P     (*(volatile uint32_t*)(USART1_BASE + 0x04))
#define USART1_BRR_P    (*(volatile uint32_t*)(USART1_BASE + 0x08))
#define USART1_CR1_P    (*(volatile uint32_t*)(USART1_BASE + 0x0C))

#define RCC_APB2ENR_USART1EN (1 << 4)

// Add to your RCC defines if not there
#define RCC_APB1ENR_USART2EN (1 << 17)

#define PA5 5
#define PA6 6
#define PA7 7

#define PA1 1
#define PA8 8
#define PA9 9

#define PB10 10
#define PA10 10
#define PA4 4

#define VALVES 3
#define VALVE_1_OUT1 PA6
#define VALVE_1_OUT2 PA7
#define VALVE_2_OUT1 PA8
#define VALVE_2_OUT2 PA9
#define VALVE_3_OUT1 PA4
#define VALVE_3_OUT2 PA10

int valve_states[VALVES] = {0, 0, 0}; // 0 = closed, 1 = open
int valve_pins[VALVES][2] = {
    {VALVE_1_OUT1, VALVE_1_OUT2},
    {VALVE_2_OUT1, VALVE_2_OUT2},
    {VALVE_3_OUT1, VALVE_3_OUT2}
};

void init_uart2(void) {
    // 1. Enable USART2 Clock
    RCC_P->RCC_APB1ENR |= RCC_APB1ENR_USART2EN;

    // 2. Configure PA2 (TX) and PA3 (RX) for Alternate Function 7
    // Note: Since you were using PA2/PA3 for other things, make sure they aren't 
    // being reset elsewhere. UART2 MUST have these pins.
    GPIOA_P->MODER &= ~((0x3 << (2*2)) | (0x3 << (3*2)));
    GPIOA_P->MODER |=  ((0x2 << (2*2)) | (0x2 << (3*2)));
    
    // Set AF7 (0b0111) for both PA2 and PA3
    GPIOA_P->AFRL &= ~((0xF << (2*4)) | (0xF << (3*4)));
    GPIOA_P->AFRL |=  ((0x7 << (2*4)) | (0x7 << (3*4)));

    // 3. Set Baud Rate
    USART2_BRR_P = 0x008B;

    // 4. Enable UART, Receiver, and Transmitter
    USART2_CR1_P |= (1 << 13) | (1 << 2) | (1 << 3); 
}

void init_uart1(void) {
    // Enable clock
    RCC_P->RCC_APB2ENR |= RCC_APB2ENR_USART1EN;

    // Set PA9 (TX) and PA10 (RX) to AF7
    GPIOA_P->MODER &= ~((0x3 << (9*2)) | (0x3 << (10*2)));
    GPIOA_P->MODER |=  ((0x2 << (9*2)) | (0x2 << (10*2)));

    GPIOA_P->AFRH &= ~((0xF << ((9-8)*4)) | (0xF << ((10-8)*4)));
    GPIOA_P->AFRH |=  ((0x7 << ((9-8)*4)) | (0x7 << ((10-8)*4)));

    // 256000 baud
    USART1_BRR_P = 0x003E;

    USART1_CR1_P |= (1 << 13) | (1 << 2) | (1 << 3);
}

// This function pauses the code until a key is pressed
char uart_read_char(void) {
    // Wait until RXNE (Read data register not empty) bit is set
    while (!(USART2_SR_P & (1 << 5)));
    return (char)(USART2_DR_P & 0xFF);
}

// Sends a single character
void uart_write_char(char c) {
    // 1. Wait until TXE (Transmit data register empty) bit is set (Bit 7)
    while (!(USART2_SR_P & (1 << 7)));
    
    // 2. Write the character to the Data Register
    USART2_DR_P = (c & 0xFF);
}

void uart_write_hex(uint8_t val) {
    char hex[] = "0123456789ABCDEF";
    uart_write_char(hex[(val >> 4) & 0xF]);
    uart_write_char(hex[val & 0xF]);
}

// Sends a null-terminated string
void uart_write_string(const char* str) {
    while (*str) {
        uart_write_char(*str++);
    }
}

void init_gpio_pin(gpio_reg_t* gpio, uint32_t pin) {
    gpio->MODER &= ~(0b11 << (pin * 2));
    gpio->MODER |= 0b1 << (pin * 2);
}

void gpio_pin_high(gpio_reg_t* gpio, uint32_t pin) {
	gpio->ODR |= (1 << pin);
}

void gpio_pin_low(gpio_reg_t* gpio, uint32_t pin) {
	gpio->ODR &= ~(1 << pin);
}

void gpio_pin_toggle(gpio_reg_t* gpio, uint32_t pin) {
	gpio->ODR ^= (1 << pin);
}

void init_gpioa_pin(uint32_t pin) {
	init_gpio_pin(GPIOA_P, pin);
}

void gpioa_pin_high(uint32_t pin) {
	gpio_pin_high(GPIOA_P, pin);
}

void gpioa_pin_low(uint32_t pin) {
	gpio_pin_low(GPIOA_P, pin);
}

void gpioa_pin_toggle(uint32_t pin) {
	gpio_pin_toggle(GPIOA_P, pin);
}

void init_gpiob_pin(uint32_t pin) {
	init_gpio_pin(GPIOB_P, pin);
}

void gpiob_pin_high(uint32_t pin) {
	gpio_pin_high(GPIOB_P, pin);
}

void gpiob_pin_low(uint32_t pin) {
	gpio_pin_low(GPIOB_P, pin);
}

void gpiob_pin_toggle(uint32_t pin) {
	gpio_pin_toggle(GPIOB_P, pin);
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

void init_tim2_pwm_ch1(void) {
    // 5. Configure TIM2 Channel 1 for PWM Mode 1
    // Clear OC1M (bits 6:4) and OC1PE (bit 3)
    TIM2_CCMR1_P &= ~((0x7 << 4) | (1 << 3));
    // Set OC1M to 110 (PWM Mode 1) and Enable Preload
    TIM2_CCMR1_P |= (0x6 << 4) | (1 << 3);

    // 6. Enable the Output for Channel 1
    TIM2_CCER_P |= (1 << 0);

    // 7. Start with a 0% duty cycle (motor off)
    TIM2_CCR1_P = 0;

    // 8. Enable TIM2 Counter
}

void init_tim2_pwm_ch2(void) {
    // 1-4. (Clock, GPIO, AF, and ARR/PSC are SHARED - only do once per timer)
    
    // 5. Configure TIM2 Channel 2 for PWM Mode 1
    // Clear OC2M (bits 14:12) and OC2PE (bit 11)
    TIM2_CCMR1_P &= ~((0x7 << 12) | (1 << 11));
    // Set OC2M to 110 (PWM Mode 1) and Enable Preload
    TIM2_CCMR1_P |= (0x6 << 12) | (1 << 11);

    // 6. Enable the Output for Channel 2 (Bit 4 in CCER)
    TIM2_CCER_P |= (1 << 4);

    // 7. Start at 0
    TIM2_CCR2_P = 0;
}

void init_tim2_pwm_ch3(void) {
    // 5. Configure TIM2 Channel 3 for PWM Mode 1
    // SWITCH TO CCMR2 register. Bits 4:6 and 3 (same positions as CH1 but in new reg)
    TIM2_CCMR2_P &= ~((0x7 << 4) | (1 << 3));
    TIM2_CCMR2_P |= (0x6 << 4) | (1 << 3);

    // 6. Enable the Output for Channel 3 (Bit 8 in CCER)
    TIM2_CCER_P |= (1 << 8);

    // 7. Start at 0
    TIM2_CCR3_P = 0;
}

void init_tim2_pwm(void) {
    // 1. Enable TIM2 clock on APB1 bus
    RCC_P->RCC_APB1ENR = (1 << 0);

    // 2. Configure TIM2 Frequency (1 kHz)
    TIM2_PSC_P = 16 - 1;   // 1 MHz timer clock (1us per tick)
    TIM2_ARR_P = 1000 - 1; // 1000 ticks = 1 ms period

    init_tim2_pwm_ch1();
    init_tim2_pwm_ch2();
    init_tim2_pwm_ch3();

    TIM2_CR1_P |= (1 << 0);
}

void init_tim2_pwm_gpioa_pin(int pin) {
    GPIOA_P->MODER &= ~(0b11 << (pin * 2));
    GPIOA_P->MODER |= (0b10 << (pin * 2));

    if (pin < 8) {
        GPIOA_P->AFRL &= ~(0xF << (pin * 4)); 
        GPIOA_P->AFRL |= (0x1 << (pin * 4));
    } else {
        // Use AFRH for pins 8-15
        GPIOA_P->AFRH &= ~(0xF << ((pin - 8) * 4)); 
        GPIOA_P->AFRH |= (0x1 << ((pin - 8) * 4));
    }
}

void init_tim2_pwm_gpiob_pin(int pin) {
    // 2. Configure the specified pin for Alternate Function mode
    // Clear the 2 bits for the pin, then set them to 0b10 (Alternate Function)
    GPIOB_P->MODER &= ~(0b11 << (pin * 2));
    GPIOB_P->MODER |= (0b10 << (pin * 2));

    if (pin < 8) {
        GPIOB_P->AFRL &= ~(0xF << (pin * 4)); 
        GPIOB_P->AFRL |= (0x1 << (pin * 4));
    } else {
        // Use AFRH for pins 8-15
        GPIOB_P->AFRH &= ~(0xF << ((pin - 8) * 4)); 
        GPIOB_P->AFRH |= (0x1 << ((pin - 8) * 4));
    }
}

void set_motor1_speed(uint16_t value) { // CH1
    if (value > 1000) value = 1000;
    TIM2_CCR1_P = value;
}

void set_motor2_speed(uint16_t value) { // CH2
    if (value > 1000) value = 1000;
    TIM2_CCR2_P = value;
}

void set_motor3_speed(uint16_t value) { // CH3
    if (value > 1000) value = 1000;
    TIM2_CCR3_P = value;
}

void set_valve(int valve_number, int state) {
    int out1_pin = valve_pins[valve_number][0];
    int out2_pin = valve_pins[valve_number][1];

    if (state == 1) {
        gpioa_pin_high(out1_pin);
        gpioa_pin_low(out2_pin);
    } 
    else if (state == -1) {
        gpioa_pin_low(out1_pin);
        gpioa_pin_high(out2_pin);
    }
    else {
        gpioa_pin_low(out1_pin);
        gpioa_pin_low(out2_pin);
    }

    valve_states[valve_number] = state;
}

void play_note(const char* note) {
    for (int i = 0; i < VALVES; i++) {
        int state = note[i] - '0';
        if (state == 0 && valve_states[i] == 1) {
            set_valve(i, -1); // open valve
        }
    }
    delay_ms(10);
    // set_motor1_speed(MOTOR_SPEED);
    for (int i = 0; i < VALVES; i++) {
            int state = note[i] - '0';
            if (state == valve_states[i]) {
                continue;
            }
            set_valve(i, state);
        }
}

#endif /* MAIN_H */
