#include "dac_util.h"
#include "stm32g4xx_hal.h"

uint32_t temp_dac_val;
float Dac_Current;

Current_DAC_HandleTypedef current_hdac;


void DAC_Init() {
    current_hdac.Channel = DAC_CHANNEL_1;
    current_hdac.hdac = &hdac1;
    current_hdac.state = DAC_OFF;
    current_hdac.DAC_Val = 0.0f;
    current_hdac.DAC_Decoder_val = 0;
    current_hdac.dma_state = DAC_OFF;
		HAL_DAC_SetValue(&hdac1, current_hdac.Channel, DAC_ALIGN_12B_R, current_hdac.DAC_Decoder_val);
		HAL_DAC_Start(&hdac1, current_hdac.Channel);
}

void DAC_SetVal(float Val) { 
    if(Val>=3.29f) Val = 3.29f;
	  else if(Val<=0.01f) Val = 0.0f;
    current_hdac.DAC_Decoder_val = (uint32_t)(Val / 3.3f * 4095);
    current_hdac.DAC_Val = Val;
	DAC1->DHR12R1= current_hdac.DAC_Decoder_val;
	SET_BIT(hdac1.Instance->SWTRIGR,DAC_SWTRIGR_SWTRIG1);
    //HAL_DAC_SetValue(&hdac1, current_hdac.Channel, DAC_ALIGN_12B_R, current_hdac.DAC_Decoder_val);
	//HAL_DAC_Start(&hdac1, current_hdac.Channel);
    current_hdac.state = DAC_ON;
    current_hdac.dma_state = DAC_OFF;
    current_hdac.error_code = 0;
}

float DAC_GetVal() {
    if (current_hdac.state == DAC_OFF) {
        return 0;
    }
    else if(current_hdac.dma_state == DAC_OFF) {
        return current_hdac.DAC_Val;
    }
    else {
        return HAL_DAC_GetValue(current_hdac.hdac, current_hdac.Channel)*3.3f/4095.0f;
    }
}

void DAC_STOP() {
    current_hdac.state = DAC_OFF;
    current_hdac.dma_state = DAC_OFF;
    HAL_DAC_Stop(current_hdac.hdac, current_hdac.Channel);
    HAL_DAC_Stop_DMA(current_hdac.hdac, current_hdac.Channel);
}

// void DAC_SetVal_DMA(uint32_t *pdata, uint32_t length){
//     current_hdac.state=DAC_ON;
//     current_hdac.dma_state = DAC_ON;
//     if (pdata == NULL || length <= 0)
//     {
//         current_hdac.error_code = 1;
//         Error_Handler();
//     }
//     else
//     {
//         HAL_DAC_Start_DMA(current_hdac.hdac, current_hdac.Channel, pdata, length, DAC_ALIGN_12B_R);
//     }
// }
