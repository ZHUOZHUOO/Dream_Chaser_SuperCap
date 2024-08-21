#include "adc_util.h"
#include "Boost_Control.h"



uint32_t Adc_Val[DECODER_ARRAY_SIZE];                   //Adc data array    
float    Adc_Sum_Val[DECODER_ARRAY_SIZE];		            //Adc decode data
float    Adc_Out_Val[10];
float    Adc_Out;
float    Ref = 3.3f;
float    Out_Correcting = 1.0;
lowpass_filterdata Adc_Out_Filter = {0.1 , 0 , 0} ;







void Adc_Decoder() {
    // memset(Adc_Sum_Val, 0, sizeof(Adc_Sum_Val));

    if(ADC_WORK_TYPE == CAP_BUCK) {
        Cap_Buck_Decoder();
    }
    else if(ADC_WORK_TYPE == CAP_BOOST) {
        Cap_Boost_Decoder();
    }


}


void Adc_Init(ADC_HandleTypeDef* phadc) {
   
	Adc_GetValue_DMA(phadc, (uint32_t *)Adc_Val, DECODER_ARRAY_SIZE);
}

float Adc_GetValue (ADC_HandleTypeDef *phadc) {
    
    HAL_ADC_Start(phadc);
    HAL_ADC_PollForConversion(phadc, 100);
    if (HAL_IS_BIT_SET(HAL_ADC_GetState(phadc), HAL_ADC_STATE_REG_EOC)) ///*!< Conversion data available on group regular */
    {
        return HAL_ADC_GetValue(phadc) * 3.3 / 4095.0f;
    }
    else
        return 0;
}

void Adc_GetValue_DMA(ADC_HandleTypeDef *phadc, uint32_t *pdata, uint32_t Length) 
{
    
    HAL_ADC_Start_DMA(phadc, pdata, Length);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *phdac)
{
    
	Adc_Decoder();
}


void Cap_Boost_Decoder(){
	
	for (int j = 0; j < DECODER_ARRAY_SIZE; j++)
	{
        Adc_Sum_Val[j]  = (float)Adc_Val[j] / 4095.0f * Ref; //µÚÈý¿é3.318
	    Adc_Out_Val[j]   = Adc_Sum_Val[j] *11.0f*Out_Correcting;
    }
    float Out_Temp = 0 ;
    for (int j = 0; j < 10; j++) {
        Out_Temp += Adc_Out_Val[j];
    }
    Adc_Out = lowpass_filter( &Adc_Out_Filter,Out_Temp/10.0f);

}
