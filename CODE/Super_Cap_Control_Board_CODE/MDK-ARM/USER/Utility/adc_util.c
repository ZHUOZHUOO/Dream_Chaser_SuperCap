#include "adc_util.h"
#include "cmsis_os.h"
#include "Cap_Buck_Control.h"

uint32_t Adc_Val[Adc_OverSampling_Times * ADC1_CHANNEL_NUM];                   //Adc data array    
float    Adc_Sum_Val[Adc_OverSampling_Times * ADC1_CHANNEL_NUM];		            //Adc decode data

uint32_t Adc_Val_2[Adc_OverSampling_Times * ADC2_CHANNEL_NUM];                   //Adc data array    
float    Adc_Sum_Val_2[Adc_OverSampling_Times * ADC2_CHANNEL_NUM];		            //Adc decode data

void Adc_Decoder(ADC_HandleTypeDef *hadc) {
    // memset(Adc_Sum_Val, 0, sizeof(Adc_Sum_Val));

    if(ADC_WORK_TYPE == CAP_BUCK)
		{
			if(hadc == BUCK_ADC_INSTANCE)
			{
        ADC_GET_Main();
			}
			else if(hadc == POWER_SWITCH_ADC_INSTANCE) 
			{
				ADC_GET_Port();
			}			
    }
}


void Adc_Init(void) { 
		HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
		osDelay(20);
    Adc_GetValue_DMA(&hadc1, (uint32_t *)Adc_Val, Adc_OverSampling_Times * ADC1_CHANNEL_NUM);
		Adc_GetValue_DMA(&hadc2, (uint32_t *)Adc_Val_2, Adc_OverSampling_Times * ADC2_CHANNEL_NUM);
		osDelay(20);
}

float Adc_GetValue (ADC_HandleTypeDef *hadc) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) ///*!< Conversion data available on group regular */
    {
        return HAL_ADC_GetValue(&hadc1) * 3.3 / 4095.0f;
    }
    else
        return 0;
}

void Adc_GetValue_DMA(ADC_HandleTypeDef *hadc, uint32_t *pdata, uint32_t Length) {
//    HAL_ADCEx_Calibration_Start(hadc);
    HAL_ADC_Start_DMA(hadc, pdata, Length);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    Adc_Decoder(hadc);
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc) {
		HAL_ADC_GetValue(hadc);
	
}
