
#include "ch32v00x_dbgmcu.h"

#define IDCODE_DEVID_MASK    ((uint32_t)0x0000FFFF)

/**
 * @brief   Returns the device revision identifier.
 * @return  Revision identifier.
 */
uint32_t DBGMCU_GetREVID(void) {
    return ((*(uint32_t *)0x1FFFF7C4) >> 16);
}

/**
 * @brief   Returns the device identifier.
 * @return  Device identifier.
 */
uint32_t DBGMCU_GetDEVID(void) {
    return ((*(uint32_t *)0x1FFFF7C4) & IDCODE_DEVID_MASK);
}

/**
 * @brief   Return the DEBUGE Control Register
 * @return  DEBUGE Control value
 */
uint32_t __get_DEBUG_CR(void) {
    uint32_t result;

    __asm volatile("csrr %0,""0x7C0" : "=r"(result));
    return (result);
}

/**
 * @brief   Set the DEBUGE Control Register
 * @param   value  - set DEBUGE Control value
 * @return  none
 */
void __set_DEBUG_CR(uint32_t value) {
    __asm volatile("csrw 0x7C0, %0" : : "r"(value));
}

/**
 * @brief   Configures the specified peripheral and low power mode behavior
 *        when the MCU under Debug mode.
 * @param   DBGMCU_Periph - specifies the peripheral and low power mode.
 *            DBGMCU_IWDG_STOP - Debug IWDG stopped when Core is halted
 *            DBGMCU_WWDG_STOP - Debug WWDG stopped when Core is halted
 *            DBGMCU_TIM1_STOP - TIM1 counter stopped when Core is halted
 *            DBGMCU_TIM2_STOP - TIM2 counter stopped when Core is halted
 *          NewState - ENABLE or DISABLE.
 * @return  none
 */
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState) {
    uint32_t val;

    if(NewState != DISABLE) {
        __set_DEBUG_CR(DBGMCU_Periph);
    }
    else {
        val = __get_DEBUG_CR();
        val &= ~(uint32_t)DBGMCU_Periph;
        __set_DEBUG_CR(val);
    }
}
/**
 * @brief   Returns the CHIP identifier.
 * @return Device identifier.
 *          ChipID List-
 *    CH32V003F4P6-0x003005x0
 *    CH32V003F4U6-0x003105x0
 *    CH32V003A4M6-0x003205x0
 *    CH32V003J4M6-0x003305x0
 */
uint32_t DBGMCU_GetCHIPID(void) {
    return(*(uint32_t*)0x1FFFF7C4);
}
