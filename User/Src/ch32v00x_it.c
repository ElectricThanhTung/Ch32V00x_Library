
#include "ch32v00x_it.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/**
 * @brief   This function handles NMI exception.
 * @return  none
 */
void NMI_Handler(void) {
}

/**
 * @brief   This function handles Hard Fault exception.
 * @return  none
 */
void HardFault_Handler(void) {

    return;
}
