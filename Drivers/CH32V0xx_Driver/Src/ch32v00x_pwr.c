
#include "ch32v00x_pwr.h"
#include "ch32v00x_rcc.h"

/* PWR registers bit mask */
/* CTLR register bit mask */
#define CTLR_DS_MASK     ((uint32_t)0xFFFFFFFD)
#define CTLR_PLS_MASK    ((uint32_t)0xFFFFFF1F)
#define AWUPSC_MASK      ((uint32_t)0xFFFFFFF0)
#define AWUWR_MASK       ((uint32_t)0xFFFFFFC0)

/**
 * @brief   Deinitializes the PWR peripheral registers to their default
 *        reset values.
 * @return  none
 */
void PWR_DeInit(void) {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/**
 * @brief   Enables or disables the Power Voltage Detector(PVD).
 * @param   NewState - new state of the PVD(ENABLE or DISABLE).
 * @return  none
 */
void PWR_PVDCmd(FunctionalState NewState) {
    if(NewState) {
        PWR->CTLR |= (1 << 4);
    }
    else {
        PWR->CTLR &= ~(1 << 4);
    }
}

/**
 * @brief   Configures the voltage threshold detected by the Power Voltage
 *        Detector(PVD).
 * @param   PWR_PVDLevel - specifies the PVD detection level
 *            PWR_PVDLevel_2V2 - PVD detection level set to 2.2V
 *            PWR_PVDLevel_2V3 - PVD detection level set to 2.3V
 *            PWR_PVDLevel_2V4 - PVD detection level set to 2.4V
 *            PWR_PVDLevel_2V5 - PVD detection level set to 2.5V
 *            PWR_PVDLevel_2V6 - PVD detection level set to 2.6V
 *            PWR_PVDLevel_2V7 - PVD detection level set to 2.7V
 *            PWR_PVDLevel_2V8 - PVD detection level set to 2.8V
 *            PWR_PVDLevel_2V9 - PVD detection level set to 2.9V
 * @return  none
 */
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel) {
    uint32_t tmpreg = 0;
    tmpreg = PWR->CTLR;
    tmpreg &= CTLR_PLS_MASK;
    tmpreg |= PWR_PVDLevel;
    PWR->CTLR = tmpreg;
}

/**
 * @brief   Enables or disables the Auto WakeUp functionality.
 * @param   NewState - new state of the Auto WakeUp functionality
 *        (ENABLE or DISABLE).
 * @return  none
 */
void PWR_AutoWakeUpCmd(FunctionalState NewState) {
    if(NewState) {
        PWR->AWUCSR |= (1 << 1);
    }
    else {
        PWR->AWUCSR &= ~(1 << 1);
    }
}

/**
 * @brief   Sets the Auto Wake up Prescaler
 * @param   AWU_Prescaler - specifies the Auto Wake up Prescaler
 *            PWR_AWU_Prescaler_1 - AWU counter clock = LSI/1
 *            PWR_AWU_Prescaler_2 - AWU counter clock = LSI/2
 *            PWR_AWU_Prescaler_4 - AWU counter clock = LSI/4
 *            PWR_AWU_Prescaler_8 - AWU counter clock = LSI/8
 *            PWR_AWU_Prescaler_16 - AWU counter clock = LSI/16
 *            PWR_AWU_Prescaler_32 - AWU counter clock = LSI/32
 *            PWR_AWU_Prescaler_64 - AWU counter clock = LSI/64
 *            PWR_AWU_Prescaler_128 - AWU counter clock = LSI/128
 *            PWR_AWU_Prescaler_256 - AWU counter clock = LSI/256
 *            PWR_AWU_Prescaler_512 - AWU counter clock = LSI/512
 *            PWR_AWU_Prescaler_1024 - AWU counter clock = LSI/1024
 *            PWR_AWU_Prescaler_2048 - AWU counter clock = LSI/2048
 *            PWR_AWU_Prescaler_4096 - AWU counter clock = LSI/4096
 *            PWR_AWU_Prescaler_10240 - AWU counter clock = LSI/10240
 *            PWR_AWU_Prescaler_61440 - AWU counter clock = LSI/61440
 * @return  none
 */
void PWR_AWU_SetPrescaler(uint32_t AWU_Prescaler) {
    uint32_t tmpreg = 0;
    tmpreg = PWR->AWUPSC & AWUPSC_MASK;
    tmpreg |= AWU_Prescaler;
    PWR->AWUPSC = tmpreg;
}

/**
 * @brief   Sets the WWDG window value
 * @param   WindowValue - specifies the window value to be compared to the
 *        downcounter,which must be lower than 0x3F
 * @return  none
 */
void PWR_AWU_SetWindowValue(uint8_t WindowValue) {
    __IO uint32_t tmpreg = 0;

    tmpreg = PWR->AWUWR & AWUWR_MASK;

    tmpreg |= WindowValue;

    PWR->AWUWR  = tmpreg;
}

/**
 * @brief   Enters STANDBY mode.
 * @param   PWR_STANDBYEntry - specifies if STANDBY mode in entered with WFI or WFE instruction.
 *            PWR_STANDBYEntry_WFI - enter STANDBY mode with WFI instruction
 *            PWR_STANDBYEntry_WFE - enter STANDBY mode with WFE instruction
 * @return  none
 */
void PWR_EnterSTANDBYMode(uint8_t PWR_STANDBYEntry) {
    PWR->CTLR &= CTLR_DS_MASK;
    PWR->CTLR |= PWR_CTLR_PDDS;

    NVIC->SCTLR |= (1 << 2);

    if(PWR_STANDBYEntry == PWR_STANDBYEntry_WFI) {
        __WFI();
    }
    else {
        __WFE();
    }

    NVIC->SCTLR &= ~(1 << 2);
}

/**
 * @brief   Checks whether the specified PWR flag is set or not.
 * @param   PWR_FLAG - specifies the flag to check.
 *            PWR_FLAG_PVDO - PVD Output
 * @return  The new state of PWR_FLAG (SET or RESET).
 */
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG) {
    FlagStatus bitstatus = RESET;

    if((PWR->CSR & PWR_FLAG) != (uint32_t)RESET) {
        bitstatus = SET;
    }
    else {
        bitstatus = RESET;
    }
    return bitstatus;
}
