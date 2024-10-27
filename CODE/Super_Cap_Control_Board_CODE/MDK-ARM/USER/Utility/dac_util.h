#ifndef __DAC_UTIL_H
#define __DAC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "dac.h"
extern uint32_t DAC_BUFF[90];

    typedef enum
    {
        DAC_OFF = 0,
        DAC_ON = 1
    } DAC_State_Current;

    typedef struct
    {
        DAC_State_Current state;
        DAC_State_Current dma_state;
        uint8_t error_code;
        DAC_HandleTypeDef *hdac;
        uint32_t Channel;
        uint32_t DAC_Decoder_val;
        float DAC_Val;
    } Current_DAC_HandleTypedef;

    extern Current_DAC_HandleTypedef current_hdac;
    extern float Dac_Current;
    
void DAC_Init(void);
void DAC_SetVal(float Val);
float DAC_GetVal(void);
void DAC_STOP(void);


// void DAC_SetVal_DMA(uint32_t *pdata, uint32_t length);
#endif
