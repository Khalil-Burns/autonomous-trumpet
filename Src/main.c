#include <stdint.h>

#include "main.h"

#define MOTOR_SPEED 450
#define TRUMPET_TEMPO_MS 100  // 500ms per note (adjust as needed)

const char* bumblebee[] = {
	Fs5, F5, E5, Ds5, E5, Ds5, D5, Cs5,
	D5, Cs5, C5, B4, C5, B4, As4, A4,

	As4, A4, Gs4, G4, Gs4, G4, Fs4, F4,

    Fs4, F4, E4, Ds4, E4, Ds4, D4, Cs4,
	Fs4, F4, E4, Ds4, E4, Ds4, D4, Cs4,

	Fs4, F4, E4, Ds4, E4, Ds4, D4, Cs4,
	Fs4, F4, E4, Ds4, E4, Ds4, D4, Cs4,

	Fs4, F4, E4, Ds4, D4, G4, Fs4, F4,
	Fs4, F4, E4, Ds4, D4, Ds4, E4, F4,

	Fs4, F4, E4, Ds4, D4, G4, Fs4, F4,
	Fs4, F4, E4, Ds4, D4, Ds4, E4, F4,

	Fs4, F4, E4, Ds4, E4, Ds4, D4, Cs4,
    D4, Ds4, E4, F4, Fs4, G4, Fs4, F5,

	Fs4, F4, E4, Ds4, E4, Ds4, D4, Cs4,
	D4, Ds4, E4, F4, Fs4, G4, Fs4, F5,
};

#define NUM_NOTES (sizeof(bumblebee) / sizeof(bumblebee[0]))

int main() {
	init_timing();

	RCC_P->RCC_AHB1ENR |= (1 << 0);
    RCC_P->RCC_AHB1ENR |= (1 << 1);

    init_tim2_pwm();
	init_tim2_pwm_gpioa_pin(PA5);
	init_tim2_pwm_gpioa_pin(PA1);
    init_tim2_pwm_gpiob_pin(PB10);
    set_motor1_speed(MOTOR_SPEED);
    set_motor2_speed(MOTOR_SPEED);
    set_motor3_speed(MOTOR_SPEED);

    init_gpioa_pin(PA4);
	init_gpioa_pin(PA6);
	init_gpioa_pin(PA7);
	init_gpioa_pin(PA8);
	init_gpioa_pin(PA9);
	init_gpioa_pin(PA10);

	while (1) {
        for (int i = 0; i < NUM_NOTES; i++) {
            play_note(bumblebee[i]);
            delay_ms(TRUMPET_TEMPO_MS);
        }
    }
}
