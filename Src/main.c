#include <stdint.h>



#define PERIPHERAL_BASE 0x40000000

#define AHB1_OFFSET 0x20000
#define AHB1_BASE (PERIPHERAL_BASE + AHB1_OFFSET)

#define RCC_OFFSET 0x3800
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)

#define RCC_AHB1ENR_OFFSET 0x30
#define RCC_AHB1ENR_P (*(volatile uint32_t*)(RCC_BASE + RCC_AHB1ENR_OFFSET))

#define CRC_OFFSET 0x23000
#define CRC_BASE (AHB1_BASE + CRC_OFFSET)

#define GPIOA_OFFSET 0x0
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)

#define GPIO_MODER_OFFSET 0x0
#define GPIOA_MODER_P (*(volatile uint32_t*)(GPIOA_BASE + GPIO_MODER_OFFSET))

#define GPIO_OUT_OFFSET 0x14
#define GPIO_OUT_P (*(volatile uint32_t*)(GPIOA_BASE + GPIO_OUT_OFFSET))

#define PIN5 (1 << 5)

#define GREEN_LED_PIN 5
#define GREEN_LED PIN5

int main() {
	RCC_AHB1ENR_P |= 0x1;

	GPIOA_MODER_P &= ~(0b11 << (GREEN_LED_PIN * 2));
	GPIOA_MODER_P |= 0b1 << (GREEN_LED_PIN * 2);

	while (1) {
		GPIO_OUT_P ^= GREEN_LED;

		for (uint32_t i = 0; i < 100000; i++);
	}
}
