#include <stdint.h>
#include "main.h"

#define FRAME_LEN 32   // enough for LD2450 frame

uint8_t buffer[FRAME_LEN];
int buf_index = 0;

// Simple blocking read of one byte
uint8_t uart_read_byte() {
    while (!(USART2_SR_P & (1 << 5)));
    return (uint8_t)(USART2_DR_P & 0xFF);
}

uint8_t uart1_read_byte() {
    while (!(USART1_SR_P & (1 << 5)));
    return (uint8_t)(USART1_DR_P & 0xFF);
}

// Print integer (quick + dirty)
void uart_write_int(int val) {
    char buf[16];
    int i = 0;

    if (val == 0) {
        uart_write_char('0');
        return;
    }

    if (val < 0) {
        uart_write_char('-');
        val = -val;
    }

    while (val > 0) {
        buf[i++] = (val % 10) + '0';
        val /= 10;
    }

    while (i--) {
        uart_write_char(buf[i]);
    }
}

void parse_target(uint8_t* data, int index) {
    // 1. Combine bytes (Little Endian)
    uint16_t x_raw = data[0] | (data[1] << 8);
    uint16_t y_raw = data[2] | (data[3] << 8);
    uint16_t speed_raw = data[4] | (data[5] << 8);
    // data[6] and data[7] are 'Resolution', we will skip them to save time

    // 2. Handle the LD2450 Sign Bit (Bit 15)
    // If bit 15 is 1, the value is negative.
    int16_t x = (x_raw & 0x8000) ? -(int16_t)(x_raw & 0x7FFF) : (int16_t)x_raw;
    int16_t y = (y_raw & 0x8000) ? -(int16_t)(y_raw & 0x7FFF) : (int16_t)y_raw;
    int16_t speed = (speed_raw & 0x8000) ? -(int16_t)(speed_raw & 0x7FFF) : (int16_t)speed_raw;

    // 3. Only print if the target is actually active (Y > 0)
    // This prevents the "All 0s" spam when no one is there
    if (y == 0 && x == 0) return;

    uart_write_string("T");
    uart_write_int(index);
    uart_write_string(" X:");
    uart_write_int(x);
    uart_write_string(" Y:");
    uart_write_int(y);
    uart_write_string(" S:");
    uart_write_int(speed);
    uart_write_string("\r\n");
}

// Try to parse a full frame
void process_frame(uint8_t* frame) {
    uart_write_string("\r\n--- FRAME ---\r\n");

    // Targets start after header (4 bytes)
    uint8_t* data = &frame[4];

    // Each target = 8 bytes
    for (int i = 0; i < 3; i++) {
        parse_target(&data[i * 8], i);
    }

    uart_write_string("-------------\r\n");
}

void print_frame(uint8_t* frame, int len) {
    uart_write_string("\r\nFRAME: ");

    for (int i = 0; i < len; i++) {
        uart_write_string("0x");
        uart_write_int(frame[i]);
        uart_write_string(" ");
    }

    uart_write_string("\r\n");
}

int main() {
    init_timing();

    // Enable GPIO + UART
    RCC_P->RCC_AHB1ENR |= (1 << 0);
    RCC_P->RCC_AHB1ENR |= (1 << 1);

    init_uart2();
    init_uart1();

    uart_write_string("LD2450 Reader Started\r\n");

    while (1) {
        if (uart1_read_byte() == 0xAA) {
            if (uart1_read_byte() == 0xFF) {
                if (uart1_read_byte() == 0x03) {
                    if (uart1_read_byte() == 0x00) {

                        // Read exactly 26 bytes (24 for targets + 2 for footer)
                        for (int i = 0; i < 26; i++) {
                            buffer[i] = uart1_read_byte();
                        }

                        // Process the 3 targets stored in buffer[0...23]
                        for (int i = 0; i < 3; i++) {
                            parse_target(&buffer[i * 8], i + 1);
                        }
                    }
                }
            }
        }
    }
}
