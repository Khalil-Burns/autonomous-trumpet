#include <stdint.h>

/* * SRAM boundaries (from the linker script usually, or the datasheet).
 * The STM32F401RE has 96KB of RAM starting at 0x20000000.
 */
#define SRAM_START  0x20000000U
#define SRAM_SIZE   (96U * 1024U) // 96KB
#define SRAM_END    ((SRAM_START) + (SRAM_SIZE))

#define STACK_START SRAM_END

/* Symbols defined in the linker script */
extern uint32_t _sdata;   // Start of .data in RAM
extern uint32_t _edata;   // End of .data in RAM
extern uint32_t _sidata; // Source address of .data in Flash (Load Address)
extern uint32_t _sbss;    // Start of .bss in RAM
extern uint32_t _ebss;    // End of .bss in RAM

// Function prototypes
void Reset_Handler(void);
void Default_Handler(void);
int main(void);

/*
 * The Vector Table
 * Placed at 0x00000000 (Flash start) via the linker script section ".isr_vector"
 */
uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
    STACK_START,        // 0x00: Initial Stack Pointer (MSP)
    (uint32_t)Reset_Handler,      // 0x04: Reset Handler (Entry Point)
    (uint32_t)Default_Handler,    // 0x08: NMI Handler
    (uint32_t)Default_Handler,    // 0x0C: HardFault Handler
    (uint32_t)Default_Handler,    // 0x10: MemManage Handler
    (uint32_t)Default_Handler,    // 0x14: BusFault Handler
    (uint32_t)Default_Handler,    // 0x18: UsageFault Handler
    0, 0, 0, 0,                   // 0x1C-0x28: Reserved
    (uint32_t)Default_Handler,    // 0x2C: SVCall Handler (Critical for RTOS!)
    (uint32_t)Default_Handler,    // 0x30: Debug Monitor
    0,                            // 0x34: Reserved
    (uint32_t)Default_Handler,    // 0x38: PendSV Handler (Critical for RTOS!)
    (uint32_t)Default_Handler,    // 0x3C: SysTick Handler (Critical for RTOS!)

    // External Interrupts would follow here...
};

/* * The Reset Handler
 * This is the first code that executes when the CPU wakes up.
 */
void Reset_Handler(void) {
    // 1. Copy .data section from Flash to RAM
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint8_t *pDst = (uint8_t*)&_sdata;      // RAM
    uint8_t *pSrc = (uint8_t*)&_sidata;    // Flash

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = *pSrc++;
    }

    // 2. Zero initialize the .bss section in RAM
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint8_t*)&_sbss;
    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = 0;
    }

    // 3. Call main()
    main();
}

/*
 * Default Handler
 * Catches any interrupt we haven't implemented yet.
 */
void Default_Handler(void) {
    while(1); // Infinite loop to catch faults
}
