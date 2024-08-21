#ifndef __ADC_UTIL_H
#define __ADC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "adc.h"


#define CHANNEL_NUM  1
#define DECODER_ARRAY_SIZE  10
#define ADC_WORK_TYPE   CAP_BOOST
#define CAP_BUCK   1
#define CAP_BOOST  2
#define REF_ADDRESS 0x08007000
#define OUT_CORRECTING_ADDRESS 0x08007B20
extern float    Ref;
extern float    Out_Correcting;


extern uint32_t Adc_Val[DECODER_ARRAY_SIZE];                   //Adc data array    
extern float    Adc_Sum_Val[DECODER_ARRAY_SIZE];		            //Adc decode data
extern float    Adc_Out;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *phdac);
float Adc_GetValue(ADC_HandleTypeDef *phadc);
void Adc_GetValue_DMA(ADC_HandleTypeDef *phadc, uint32_t *pdata, uint32_t Length);
void Adc_Init(ADC_HandleTypeDef *phadc);
void Adc_Decoder(void) ;
void Cap_Buck_Decoder(void);
void Cap_Boost_Decoder(void);

#endif
