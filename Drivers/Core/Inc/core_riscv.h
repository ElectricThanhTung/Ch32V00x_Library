
#ifndef __CORE_RISCV_H
#define __CORE_RISCV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* IO definitions */
#ifdef __cplusplus
  #define     __I               volatile            /*  defines 'read only' permissions     */
#else
  #define     __I               volatile const      /*  defines 'read only' permissions     */
#endif
#define       __O               volatile            /*  defines 'write only' permissions    */
#define       __IO              volatile            /*  defines 'read / write' permissions  */

#if defined(__CC_ARM)
  #define __ASM                 __asm               /*!< asm keyword for ARM Compiler          */
  #define __INLINE              __inline            /*!< inline keyword for ARM Compiler       */
#elif defined(__ICCARM__)
  #define __ASM                 __asm               /*!< asm keyword for IAR Compiler          */
  #define __INLINE              inline              /*!< inline keyword for IAR Compiler. Only avaiable in High optimization mode! */
#elif defined(__GNUC__)
  #define __ASM                 __asm               /*!< asm keyword for GNU Compiler          */
  #define __INLINE              inline              /*!< inline keyword for GNU Compiler       */
#elif defined(__TASKING__)
  #define __ASM                 __asm               /*!< asm keyword for TASKING Compiler      */
  #define __INLINE              inline              /*!< inline keyword for TASKING Compiler   */
#endif

#define __STATIC_FORCEINLINE    __attribute__((always_inline)) static inline

typedef enum {
    NoREADY = 0,
    READY = !NoREADY
} ErrorStatus;

typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

typedef enum {
    RESET = 0,
    SET = !RESET
} FlagStatus, ITStatus;

/* memory mapped structure for Program Fast Interrupt Controller (PFIC) */
typedef struct {
    __I  uint32_t ISR[8];
    __I  uint32_t IPR[8];
    __IO uint32_t ITHRESDR;
    __IO uint32_t RESERVED;
    __IO uint32_t CFGR;
    __I  uint32_t GISR;
    __IO uint8_t VTFIDR[4];
    uint8_t RESERVED0[12];
    __IO uint32_t VTFADDR[4];
    uint8_t RESERVED1[0x90];
    __O  uint32_t IENR[8];
    uint8_t RESERVED2[0x60];
    __O  uint32_t IRER[8];
    uint8_t RESERVED3[0x60];
    __O  uint32_t IPSR[8];
    uint8_t RESERVED4[0x60];
    __O  uint32_t IPRR[8];
    uint8_t RESERVED5[0x60];
    __IO uint32_t IACTR[8];
    uint8_t RESERVED6[0xE0];
    __IO uint8_t IPRIOR[256];
    uint8_t RESERVED7[0x810];
    __IO uint32_t SCTLR;
} PFIC_Type;

/* memory mapped structure for SysTick */
typedef struct {
    __IO uint32_t CTLR;
    __IO uint32_t SR;
    __IO uint32_t CNT;
    uint32_t RESERVED0;
    __IO uint32_t CMP;
    uint32_t RESERVED1;
} SysTick_Type;

#define PFIC            ((PFIC_Type *)0xE000E000)
#define NVIC            PFIC
#define NVIC_KEY1       ((uint32_t)0xFA050000)
#define	NVIC_KEY2	      ((uint32_t)0xBCAF0000)
#define	NVIC_KEY3       ((uint32_t)0xBEEF0000)

#define SysTick         ((SysTick_Type *) 0xE000F000)

/**
 * @brief   Enable Global Interrupt
 * @return  none
 */
__STATIC_FORCEINLINE void __enable_irq(void) {
    uint32_t result;
    __asm volatile("csrr %0, mstatus" : "=r"(result));
    result |= 0x88;
    __asm volatile ("csrw mstatus, %0" : : "r" (result) );
}

/**
 * @brief   Disable Global Interrupt
 * @return  none
 */
__STATIC_FORCEINLINE void __disable_irq(void) {
    uint32_t result;
    __asm volatile("csrr %0, mstatus" : "=r"(result));
    result &= ~0x88;
    __asm volatile ("csrw mstatus, %0" : : "r" (result) );
}

/**
 * @brief   nop
 * @return  none
 */
__STATIC_FORCEINLINE void __NOP(void) {
    __asm volatile ("nop");
}

/**
 * @brief   Disable Interrupt
 * @param   IRQn - Interrupt Numbers
 * @return  none
 */
__STATIC_FORCEINLINE void NVIC_EnableIRQ(IRQn_Type IRQn) {
    NVIC->IENR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/**
 * @brief   Disable Interrupt
 * @param   IRQn - Interrupt Numbers
 * @return  none
 */
__STATIC_FORCEINLINE void NVIC_DisableIRQ(IRQn_Type IRQn) {
    NVIC->IRER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/**
 * @brief   Get Interrupt Enable State
 * @param   IRQn - Interrupt Numbers
 * @return  1 - Interrupt Pending Enable
 *          0 - Interrupt Pending Disable
 */
__STATIC_FORCEINLINE uint32_t NVIC_GetStatusIRQ(IRQn_Type IRQn) {
    return((uint32_t) ((NVIC->ISR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/**
 * @brief   Get Interrupt Pending State
 * @param   IRQn - Interrupt Numbers
 * @return  1 - Interrupt Pending Enable
 *          0 - Interrupt Pending Disable
 */
__STATIC_FORCEINLINE uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn) {
    return((uint32_t) ((NVIC->IPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/**
 * @brief   Set Interrupt Pending
 * @param   IRQn - Interrupt Numbers
 * @return  none
 */
__STATIC_FORCEINLINE void NVIC_SetPendingIRQ(IRQn_Type IRQn) {
  NVIC->IPSR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/**
 * @brief   Clear Interrupt Pending
 * @param   IRQn - Interrupt Numbers
 * @return  none
 */
__STATIC_FORCEINLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn) {
    NVIC->IPRR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/**
 * @brief   Get Interrupt Active State
 * @param   IRQn - Interrupt Numbers
 * @return  1 - Interrupt Active
 *          0 - Interrupt No Active
 */
__STATIC_FORCEINLINE uint32_t NVIC_GetActive(IRQn_Type IRQn) {
    return((uint32_t)((NVIC->IACTR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/**
 * @brief   Set Interrupt Priority
 * @param   IRQn - Interrupt Numbers
 *          priority: bit[7] - pre-emption priority
 *                    bit[6] - subpriority
 *                    bit[5:0] - reserved
 * @return  none
 */
__STATIC_FORCEINLINE void NVIC_SetPriority(IRQn_Type IRQn, uint8_t priority) {
    NVIC->IPRIOR[(uint32_t)(IRQn)] = priority;
}

/**
 * @brief   Wait for Interrupt
 * @return  none
 */
__STATIC_FORCEINLINE void __WFI(void) {
    NVIC->SCTLR &= ~(1<<3);   // wfi
    asm volatile ("wfi");
}

/**
 * @brief   Set Event
 * @return  none
 */
__STATIC_FORCEINLINE void _SEV(void) {
    uint32_t t;
    t = NVIC->SCTLR;
    NVIC->SCTLR |= (1<<3)|(1<<5);
    NVIC->SCTLR = (NVIC->SCTLR & ~(1<<5)) | (t & (1<<5));
}

/**
 * @brief   Wait for Events
 * @return  none
 */
__STATIC_FORCEINLINE void _WFE(void) {
    NVIC->SCTLR |= (1<<3);
    asm volatile ("wfi");
}

/**
 * @brief   Wait for Events
 * @return  none
 */
__STATIC_FORCEINLINE void __WFE(void) {
    _SEV();
    _WFE();
    _WFE();
}

/**
 * @brief   Set VTF Interrupt
 * @param   addr - VTF interrupt service function base address.
 *                 IRQn - Interrupt Numbers
 *                 num - VTF Interrupt Numbers
 *                 NewState -  DISABLE or ENABLE
 * @return  none
 */
__STATIC_FORCEINLINE void SetVTFIRQ(uint32_t addr, IRQn_Type IRQn, uint8_t num, FunctionalState NewState) {
    if(num > 1)
        return;
    if(NewState != DISABLE) {
        NVIC->VTFIDR[num] = IRQn;
        NVIC->VTFADDR[num] = ((addr&0xFFFFFFFE)|0x1);
    }
    else {
        NVIC->VTFIDR[num] = IRQn;
        NVIC->VTFADDR[num] = ((addr&0xFFFFFFFE)&(~0x1));
    }
}

/**
 * @brief   Initiate a system reset request
 * @return  none
 */
__STATIC_FORCEINLINE void NVIC_SystemReset(void) {
    NVIC->CFGR = NVIC_KEY3|(1<<7);
}

/**
 * @brief   Return the Machine Status Register
 * @return  mstatus value
 */
__STATIC_FORCEINLINE uint32_t __get_MSTATUS(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mstatus" : "=r"(result));
    return (result);
}

/**
 * @brief   Set the Machine Status Register
 * @param   value  - set mstatus value
 * @return  none
 */
__STATIC_FORCEINLINE void __set_MSTATUS(uint32_t value) {
    __ASM volatile("csrw mstatus, %0" : : "r"(value));
}

/**
 * @brief   Return the Machine ISA Register
 * @return  misa value
 */
__STATIC_FORCEINLINE uint32_t __get_MISA(void) {
    uint32_t result;
    __ASM volatile("csrr %0, misa" : "=r"(result));
    return (result);
}

/**
 * @brief   Set the Machine ISA Register
 * @param   value - set misa value
 * @return  none
 */
__STATIC_FORCEINLINE void __set_MISA(uint32_t value) {
    __ASM volatile("csrw misa, %0" : : "r"(value));
}

/**
 * @brief   Return the Machine Trap-Vector Base-Address Register
 * @return  mtvec value
 */
__STATIC_FORCEINLINE uint32_t __get_MTVEC(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mtvec" : "=r"(result));
    return (result);
}

/**
 * @brief   Set the Machine Trap-Vector Base-Address Register
 * @param   value  - set mtvec value
 * @return  none
 */
__STATIC_FORCEINLINE void __set_MTVEC(uint32_t value) {
    __ASM volatile("csrw mtvec, %0" : : "r"(value));
}

/**
 * @brief   Return the Machine Seratch Register
 * @return  mscratch value
 */
__STATIC_FORCEINLINE uint32_t __get_MSCRATCH(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mscratch" : "=r"(result));
    return (result);
}

/**
 * @brief   Set the Machine Seratch Register
 * @param   value  - set mscratch value
 * @return  none
 */
__STATIC_FORCEINLINE void __set_MSCRATCH(uint32_t value) {
    __ASM volatile("csrw mscratch, %0" : : "r"(value));
}

/**
 * @brief   Return the Machine Exception Program Register
 * @return  mepc value
 */
__STATIC_FORCEINLINE uint32_t __get_MEPC(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mepc" : "=r"(result));
    return (result);
}

/**
 * @brief   Set the Machine Exception Program Register
 * @return  none
 */
__STATIC_FORCEINLINE void __set_MEPC(uint32_t value) {
    __ASM volatile("csrw mepc, %0" : : "r"(value));
}

/**
 * @brief   Return the Machine Cause Register
 * @return  mcause value
 */
__STATIC_FORCEINLINE uint32_t __get_MCAUSE(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mcause" : "=r"(result));
    return (result);
}

/**
 * @brief   Set the Machine Cause Register
 * @return  none
 */
__STATIC_FORCEINLINE void __set_MCAUSE(uint32_t value) {
    __ASM volatile("csrw mcause, %0" : : "r"(value));
}

/**
 * @brief   Return Vendor ID Register
 * @return  mvendorid value
 */
__STATIC_FORCEINLINE uint32_t __get_MVENDORID(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mvendorid" : "=r"(result));
    return (result);
}

/**
 * @brief   Return Machine Architecture ID Register
 * @return  marchid value
 */
__STATIC_FORCEINLINE uint32_t __get_MARCHID(void) {
    uint32_t result;
    __ASM volatile("csrr %0, marchid" : "=r"(result));
    return (result);
}

/**
 * @brief   Return Machine Implementation ID Register
 * @return  mimpid value
 */
__STATIC_FORCEINLINE uint32_t __get_MIMPID(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mimpid" : "=r"(result));
    return (result);
}

/**
 * @brief   Return Hart ID Register
 * @return  mhartid value
 */
__STATIC_FORCEINLINE uint32_t __get_MHARTID(void) {
    uint32_t result;
    __ASM volatile("csrr %0, mhartid" : "=r"(result));
    return (result);
}

/**
 * @brief   Return SP Register
 * @return  SP value
 */
__STATIC_FORCEINLINE uint32_t __get_SP(void) {
    uint32_t result;
    __ASM volatile("mv %0, sp" : "=r"(result));
    return (result);
}

/**
 * @brief   Set SP Register
 * @param   value  - set sp value
 * @return  none
 */
__STATIC_FORCEINLINE void __set_SP(uint32_t value) {
    __ASM volatile("mv sp, %0" : : "r"(value));
}

#ifdef __cplusplus
}
#endif

#endif/* __CORE_RISCV_H */
