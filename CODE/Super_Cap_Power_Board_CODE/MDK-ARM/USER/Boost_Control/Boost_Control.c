#include"Boost_Control.h"
#include"AD8402_Periph.h"
#include"adc_util.h"
#include"cmsis_os.h"
#include"referee_dev.h"
#include "flash_util.h"

float Boost_AD8402_Value[2] = { 10.0f, 2.0f } ;
float Boost_AD8402_LastValue[2] = { 0 , 0 } ;
Boost_CtrTypedef Boost_BoostCtrState ;
uint8_t data[8] = {0};
long commu_flag = 0;

void BoostControlTask(void const * argument)
{
		Can_IntFilterAndStart(&hcan) ;
		Boost_Control_Start_Output();
		AD8402_RESET();
	  Boost_BoostCtrState.voltage = 24.0f;
		Boost_AD8402_Value[1] = Boost_Voltage_Decoder(Boost_BoostCtrState.voltage);
		Cap_Control_Soft_Start();
		
		for(ever) {
			if(Boost_BoostCtrState.sat == BOOST_WORK_DISABLE||commu_flag<=-100) {	
				Boost_BoostCtrState.voltage = 6.0f;
				Boost_AD8402_Value[1] =  Boost_Voltage_Decoder(Boost_BoostCtrState.voltage);
				AD8402_SetValue(Boost_AD8402_Value);
				Boost_Control_Stop_Output();
			}
			else {
   		  Boost_AD8402_Value[1] = Boost_Voltage_Decoder(Boost_BoostCtrState.voltage);
				AD8402_SetValue(Boost_AD8402_Value) ;
				Boost_Control_Start_Output();
			}
			if(commu_flag<=-10000) 
			{
				commu_flag = -200;
			}
			
			commu_flag--;
			osDelay(2);
		}

}

void LedShineTask(void const * argument)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);

	for(ever)
	{
		if(commu_flag<= 0) 
		{
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
		osDelay(50);
		}
		else 
		{
			if(Boost_BoostCtrState.voltage>28)
			{
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
			osDelay(1000);
			}
			else{
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
			osDelay(500);
			}
		}
	}
}

void AdcOutTask(void const * argument)
{
	  
	Adc_Init(&hadc);
	HAL_ADCEx_Calibration_Start(&hadc);
	osDelay(20);
  	for(ever) {
		Adc_Init(&hadc);
    	osDelay(1);
    }

}
void Can_SendMessage (void) {

		CAN_TxHeaderTypeDef pheader;
		Can_InitTxHander(&pheader,CAN_ID_STD , 0x300, 8, 0);
		pheader.TransmitGlobalTime = DISABLE;
		uint8_t txdata[8];
		txdata[0] = 0xE1 ;
		txdata[0] = (uint8_t)0xE1;
		txdata[1] = *((uint8_t*)&Adc_Out);
		txdata[2] = *((uint8_t*)&Adc_Out+1);
		txdata[3] = *((uint8_t*)&Adc_Out+2);
		txdata[4] = *((uint8_t*)&Adc_Out+3);
		txdata[5] = 0 ;
		txdata[6] = 0 ;
		txdata[7] = 66 ;
		uint32_t mailbox;
		HAL_CAN_AddTxMessage(&hcan, &pheader, txdata, &mailbox);
}

void Boost_Control_Decoder( CAN_HandleTypeDef *phcan ) {

    CAN_RxHeaderTypeDef rxhander;
    uint8_t Can_RxData[8];
    while ( HAL_CAN_GetRxMessage(phcan, CAN_RX_FIFO0, &rxhander, Can_RxData)!= HAL_OK) ;
    switch(rxhander.StdId) {
    case 0x297:
			
    Boost_CommunicatorDecoder(Can_RxData);
		break;
  }
}

void Boost_CommunicatorDecoder(uint8_t* rxdata) {
	
		data[0]  =  rxdata[0];
		data[1]  =  rxdata[1];
		data[2]  =  rxdata[2];
		data[3]  =  rxdata[3];
		data[4]  =  rxdata[4];
		data[5]  =  rxdata[5];
		data[6]  =  rxdata[6];
		data[7]  =  rxdata[7];

    if( rxdata[0] == SELF_ID ) { //该板的ID在Boost_Control.h文件的宏定义中
    	if(rxdata[7] != 66) {
      	RxError_Handler() ;	
      	return ;
    } 
//  Boost_BoostCtrState.voltage = *(float*)(&rxdata[1]) ;
		float voltage_val = 0;
		*((uint8_t*)(&voltage_val))   = rxdata[1];
		*((uint8_t*)(&voltage_val)+1) = rxdata[2];
		*((uint8_t*)(&voltage_val)+2) = rxdata[3];
		*((uint8_t*)(&voltage_val)+3) = rxdata[4];
	    Boost_BoostCtrState.voltage   = voltage_val ;
        Boost_BoostCtrState.mode      = rxdata[5] ;
        Boost_BoostCtrState.sat       = rxdata[6] ; 
		commu_flag = 200;
  }
}

void  RxError_Handler() 
{
		while (1)
		{

		}
}

float Boost_Voltage_Decoder(float exp_val) {

  	float AD8402_Value = 0;
  	AD8402_Value = (1.2f* (RES_DOWN + RES_UP) - exp_val * RES_DOWN) / (exp_val-1.20f) ;
  	return AD8402_Value;  
}

float lowpass_filter(lowpass_filterdata * pdata ,float pvalue) {
	
		float temp_value ;
		pdata->value = pvalue;
		temp_value = pdata->lowpass_param *pdata->value + (1-pdata->lowpass_param)* pdata->last_value ;
		pdata->last_value = temp_value ;
		return temp_value ;
}

void Cap_Control_Soft_Start(void) {
	uint8_t sum_value= AD8402_DecoderValue(Boost_AD8402_Value[1]);
	uint8_t temp_value = 255;
	while(temp_value>=sum_value)
	{
			AD8402_Periph_DATA[1] = 0x01 << 8 | temp_value ;
	    HAL_SPI_Transmit(&hspi1, (uint8_t*)AD8402_Periph_DATA, 2, 100);
		  temp_value--;
			for(int i = 0;i<=20;i++);
	}
}

void Boost_Control_Stop_Output(void) {
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
}

void Boost_Control_Start_Output(void) {
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
}






