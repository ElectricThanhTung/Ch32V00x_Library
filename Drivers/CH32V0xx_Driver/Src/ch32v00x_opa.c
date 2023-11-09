
#include "ch32v00x_opa.h"

/**
 * @brief   Deinitializes the OPA peripheral registers to their default
 *        reset values.
 * @return  none
 */
void OPA_DeInit(void) {
    EXTEN->EXTEN_CTR &= ~(uint32_t)(7 << 16);
}

/**
 * @brief   Initializes the OPA peripheral according to the specified
 *        parameters in the OPA_InitStruct.
 * @param   OPA_InitStruct - pointer to a OPA_InitTypeDef structure
 * @return  none
 */
void OPA_Init(OPA_InitTypeDef *OPA_InitStruct) {
    uint32_t tmp = 0;
    tmp = EXTEN->EXTEN_CTR;
    tmp &= ~(uint32_t)(3<<17);
    tmp |= (OPA_InitStruct->PSEL << 18) | (OPA_InitStruct->NSEL << 17);
    EXTEN->EXTEN_CTR = tmp;
}

/**
 * @brief   Fills each OPA_StructInit member with its reset value.
 * @param   OPA_StructInit - pointer to a OPA_InitTypeDef structure
 * @return  none
 */
void OPA_StructInit(OPA_InitTypeDef *OPA_InitStruct) {
    OPA_InitStruct->PSEL = CHP0;
    OPA_InitStruct->NSEL = CHN0;
}

/**
 * @brief   Enables or disables the specified OPA peripheral.
 * @param   OPA_NUM - Select OPA
 *            NewState - ENABLE or DISABLE.
 * @return  none
 */
void OPA_Cmd(FunctionalState NewState) {
    if(NewState == ENABLE) {
        EXTEN->EXTEN_CTR |= (uint32_t)(1 << 16);
    }
    else {
        EXTEN->EXTEN_CTR &= ~(uint32_t)(1 << 16);
    }
}
