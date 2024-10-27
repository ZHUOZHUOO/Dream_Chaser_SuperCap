#include "Cap_Main_Control.h"
#include "Cap_Buck_Control.h"
#include "cmsis_os.h"
#include "Cap_Communite.h"
#include "uart_util.h"
#include "fdcan_util.h"
#include "iwdg.h"

void Task_Ledshine(void const * argument)
{
  /* USER CODE BEGIN Task_Ledshine */

  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
		if(Cap_Control_State.Communicate_State == Communicate_Offline)
		{
			osDelay(80);
		}
		else if(Cap_Adc_Data.Adc_Cap<=12.0f) 
		{
			osDelay(40);
		}
		else if(Cap_Adc_Data.Adc_Cap>=12.0f && Cap_Adc_Data.Adc_Cap<=26)
		{
			osDelay(500);
		}
		else if(Cap_Adc_Data.Adc_Cap>26) 
		{
			osDelay(200);
		}
  }
  /* USER CODE END Task_Ledshine */
}

 uint8_t Uart2_Board_Community_Buffer[24];
 uint8_t Uart3_Board_Community_Buffer[24];

void Task_Communite(void const * argument)
{
  /* USER CODE BEGIN Task_Communite */
  /* Infinite loop */
	Uart_DMA_Init(&huart3);
	Uart_ReceiveDMA(&huart3,Uart3_Board_Community_Buffer,24);
	FDCAN_IntFilterAndStart(&hfdcan1);
	
	for(ever) 
	{
		if(Cap_Control_State.Communicate_State == Communicate_Online)
		{	
		  Cap_Control_State.System_sum_Power  =  Cap_Adc_Data.Motor_Power + Cap_Adc_Data.Cap_Charging_Power;
		  Cap_Control_State.Cap_Remain_Energy = (int)((Cap_Adc_Data.Adc_Cap/CAP_MAX_VOLTAGE)*(Cap_Adc_Data.Adc_Cap/CAP_MAX_VOLTAGE)*100);
		  Cap_SendMessage_Can();
		}
		IWDG_State.Task_Communite_Down = 1;
		osDelay(50);
	}
  /* USER CODE END Task_Communite */
}

void Cap_SendMessage_Can(void)
{
	uint8_t txdata_1[8];
	txdata_1[0] = *((uint8_t*)&Cap_Adc_Data.Motor_Power);
	txdata_1[1] = *((uint8_t*)&Cap_Adc_Data.Motor_Power+1);
	txdata_1[2] = *((uint8_t*)&Cap_Adc_Data.Motor_Power+2);
	txdata_1[3] = *((uint8_t*)&Cap_Adc_Data.Motor_Power+3);
	txdata_1[4] = Cap_Control_State.Cap_Remain_Energy;

	FDCAN_SendMessageWithOutBaudSwitch(&hfdcan1,txdata_1,FDCAN_DLC_BYTES_8,0x299);

}

void Cap_Communicator_toBoss(uint8_t* rxdata) 
{
	uint8_t Cap_State_temp = Cap_Control_State.Cap_State;
	if(rxdata[0]>>1 != 0x77) {
		return;
	}
	
	Cap_Control_State.Communicate_Last_Tick = HAL_GetTick();
	Cap_Control_State.Cap_State = (rxdata[0]&0x01) | Cap_Control_State.Cap_Must_En | Cap_Control_State.Cap_Down_En;
	if(Cap_State_temp == 1 && Cap_Control_State.Cap_State == 0)
	{
		Cap_Control_State.Cap_Disable_Tick = HAL_GetTick();
	}
	if(Cap_Control_State.Cap_State == 1 && Cap_State_temp == 0)
	{
		Cap_Control_State.Cap_Enable_Tick= HAL_GetTick();
	}
	uint8_t boost_state_temp;
	boost_state_temp = rxdata[1]& 0x0001 ;
	Cap_Control_State.Chassis_Starting_State = ((rxdata[1]&0x04)>>2) ;
	Cap_Control_State.Chassis_Down_State = ((rxdata[1]& 0x02)>>1) ;
	Cap_Control_State.Motor_Safety_State = ((rxdata[1]&0x02)>>1); 
  
	Cap_Control_State.Chassis_Power_State = ((rxdata[2]&0x04)>>2) || (Cap_Adc_Data.Adc_Vcc>=20.0f);
	if(boost_state_temp == 1 && Cap_Adc_Data.Adc_Cap>= 9.0f) Cap_Control_State.Boost_val = 27.5f;
	else Cap_Control_State.Boost_val = 24.0f;
	uint8_t temp_limit = Cap_Control_State.Power_Limit;
 	Cap_Control_State.Power_Limit = rxdata[2];	
	if(temp_limit!= Cap_Control_State.Power_Limit)
	{
		Cap_Control_State.Cap_Control_Power_Max = Cap_Control_State.Power_Limit;
	}
	Cap_Control_State.buff_power  = rxdata[3];
	Cap_Control_State.ref_power   = *((float*)(&(rxdata[4])));
}

