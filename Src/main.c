#include <stdint.h>

#include "main.h"

#define PA5 5
#define PA6 6
#define PA7 7

void init_pwm_pa5(void) {
    // 1. Enable TIM2 clock on APB1 bus
    RCC_P->RCC_APB1ENR |= (1 << 0);

    // 2. Configure PA5 for Alternate Function mode
    // Clear the 2 bits for PA5, then set them to 0b10 (Alternate Function)
    GPIOA_P->MODER &= ~(0b11 << (PA5 * 2));
    GPIOA_P->MODER |= (0b10 << (PA5 * 2));

    // 3. Connect PA5 to AF1 (TIM2_CH1)
    // The AFRL register handles pins 0-7 (4 bits per pin). PA5 is bits 20-23.
    // AF1 is 0b0001
    GPIOA_P->AFRL &= ~(0xF << (PA5 * 4)); 
    GPIOA_P->AFRL |= (0x1 << (PA5 * 4));

    // 4. Configure TIM2 Frequency (1 kHz)
    TIM2_PSC_P = 16 - 1;   // 1 MHz timer clock (1us per tick)
    TIM2_ARR_P = 1000 - 1; // 1000 ticks = 1 ms period

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
    TIM2_CR1_P |= (1 << 0);
}

// Accepts a value from 0 to 1000
void set_motor_speed(uint16_t value) {
    if (value > 1000) value = 1000;
    
    // Convert 0-100% into 0-1000 ticks
    uint32_t ticks = value;
    
    // Write directly to the capture/compare register
    TIM2_CCR1_P = ticks;
}

int main() {
	init_timing();

	RCC_P->RCC_AHB1ENR |= 0x1;

	init_pwm_pa5();
//	init_gpio_pin(PA5);
	init_gpio_pin(PA6);
	init_gpio_pin(PA7);

	gpio_pin_high(PA6);
	gpio_pin_low(PA7);
//	gpio_pin_high(PA5);

	// while (1) {
	// 	gpio_pin_toggle(PA5);

	// 	delay_ms(500);

	// 	gpio_pin_toggle(PA5);

	// 	delay_ms(500);
	// }
	while (1) {
        // Ramp up the motor
		gpio_pin_high(PA6);
		gpio_pin_low(PA7);
        set_motor_speed(350); // 25% power
        delay_s(2);
        gpio_pin_low(PA6);
        gpio_pin_high(PA7);
        delay_ms(20);
        set_motor_speed(0);
        delay_s(1);
//
//        set_motor_speed(150); // 50% power
//        delay_s(2);
//
//        set_motor_speed(200); // 100% power
//        delay_s(2);
//
//        // Stop the motor
//        set_motor_speed(250);
//        delay_s(3);
    }
}
