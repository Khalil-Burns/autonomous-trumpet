#include <stdint.h>
#include <stddef.h>

#include "main.h"

#define MOTOR_SPEED 450U
#define TRUMPET_TEMPO_MS 150U

/* NOTE: trumpet note symbols (Fs5, F5 ...) are defined in trumpet_notes.h */
static const char* const bumblebee[] = {
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
    D4, Ds4, E4, F4, Fs4, Gs4, A4, As4,
    B4, As4, A4, Gs4, G4, C5, B4, As4,
    B4, As4, A4, Gs4, G4, Gs4, A4, As4,
    B4, As4, A4, Gs4, G4, C5, B4, As4,
    B4, As4, A4, Gs4, G4, Gs4, A4, As4,
    B4, As4, A4, Gs4, A4, Gs4, G4, Fs4,
    G4, Gs4, A4, As4, B4, C5, B4, As4,
    B4, As4, A4, Gs4, A4, Gs4, G4, Fs4,
    G4, Gs4, A4, As4, B4, C5, B4, As4,
    B4
};

static const size_t NUM_NOTES = sizeof(bumblebee) / sizeof(bumblebee[0]);

/* Define globals declared in header */
int valve_states[VALVES] = {0, 0, 0};
int valve_pins[VALVES][2] = {
    {VALVE_1_OUT1, VALVE_1_OUT2},
    {VALVE_2_OUT1, VALVE_2_OUT2},
    {VALVE_3_OUT1, VALVE_3_OUT2}
};

/* -- Local helpers -- */
static void enable_gpio_clocks(void) {
    RCC_P->RCC_AHB1ENR |= (1U << 0); /* GPIOA */
    RCC_P->RCC_AHB1ENR |= (1U << 1); /* GPIOB */
}

int main(void) {
    init_timing();

    enable_gpio_clocks();

    init_tim2_pwm();
    init_tim2_pwm_gpioa_pin(PA5);
    init_tim2_pwm_gpioa_pin(PA1);
    init_tim2_pwm_gpiob_pin(PB10);
    set_motor1_speed(MOTOR_SPEED);
    set_motor2_speed(MOTOR_SPEED);
    set_motor3_speed(MOTOR_SPEED);

    init_uart2();

    init_gpioa_pin(PA4);
    init_gpioa_pin(PA6);
    init_gpioa_pin(PA7);
    init_gpioa_pin(PA8);
    init_gpioa_pin(PA9);
    init_gpioa_pin(PA10);

    uart_write_string("Initialization complete.\n");

    while (1) {
        for (size_t i = 0; i < NUM_NOTES; ++i) {
            /* Wait for user input before advancing */
            (void)uart_read_char();

            uart_write_string("Playing note: ");
            uart_write_string(bumblebee[i]);
            uart_write_string("\n");

            play_note(bumblebee[i]);

            delay_ms(TRUMPET_TEMPO_MS);
        }
    }

    /* should never reach here */
    return 0;
}
