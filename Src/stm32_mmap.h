#ifndef STM32_MMAP_H
#define STM32_MMAP_H

#include <stdint.h>

typedef struct {
	uint32_t MODER;
	uint32_t OTYPER;
	uint32_t OSPEEDR;
	uint32_t PUPDR;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;
} gpio_reg_t;

typedef struct {
	uint32_t RCC_CR;
	uint32_t RCC_PLLCFGR;
	uint32_t RCC_CFGR;
	uint32_t RCC_CIR;
	uint32_t RCC_AHB1RSTR;
	uint32_t RCC_AHB2RSTR;
	const uint32_t RESERVED1;
	const uint32_t RESERVED2;
	uint32_t RCC_APB1RSTR;
	uint32_t RCC_APB2RSTR;
	const uint32_t RESERVED3;
	const uint32_t RESERVED4;
	uint32_t RCC_AHB1ENR;
	uint32_t RCC_AHB2ENR;
	const uint32_t RESERVED5;
	const uint32_t RESERVED6;
	uint32_t RCC_APB1ENR;
	uint32_t RCC_APB2ENR;
	const uint32_t RESERVED7;
	const uint32_t RESERVED8;
	uint32_t RCC_AHB1LPENR;
	uint32_t RCC_AHB2LPENR;
	const uint32_t RESERVED9;
	const uint32_t RESERVED10;
	uint32_t RCC_APB1LPENR;
	uint32_t RCC_APB2LPENR;
	const uint32_t RESERVED11;
	const uint32_t RESERVED12;
	uint32_t RCC_BDCR;
	uint32_t RCC_CSR;
	const uint32_t RESERVED13;
	const uint32_t RESERVED14;
	uint32_t RCC_SSCGR;
	uint32_t RCC_PLLI2SCFGR;
	const uint32_t RESERVED15;
	uint32_t RCC_DCKCFGR;
} rcc_reg_t;

#endif /* STM32_MMAP_H */
