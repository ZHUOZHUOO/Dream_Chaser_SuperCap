#ifndef __ADC_UTIL_H
#define __ADC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "adc.h"

#define Adc_OverSampling_Times  1
#define ADC1_CHANNEL_NUM  6
#define ADC2_CHANNEL_NUM  3

#define ADC_WORK_TYPE   CAP_BUCK
#define CAP_BUCK   1
#define CAP_BOOST  2

#define BUCK_ADC_INSTANCE          &hadc1
#define POWER_SWITCH_ADC_INSTANCE  &hadc2

extern uint32_t Adc_Val[Adc_OverSampling_Times * ADC1_CHANNEL_NUM];                   //Adc data array    
extern float    Adc_Sum_Val[Adc_OverSampling_Times * ADC1_CHANNEL_NUM];		            //Adc decode data

extern uint32_t Adc_Val_2[ADC2_CHANNEL_NUM * Adc_OverSampling_Times];                   //Adc data array    
extern float    Adc_Sum_Val_2[ADC2_CHANNEL_NUM * Adc_OverSampling_Times];		            //Adc decode data




void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hdac);
float Adc_GetValue(ADC_HandleTypeDef *hadc);
void Adc_GetValue_DMA(ADC_HandleTypeDef *hadc, uint32_t *pdata, uint32_t Length);
void Adc_Init(void);
void Adc_Decoder(ADC_HandleTypeDef *hadc) ;

#endif
