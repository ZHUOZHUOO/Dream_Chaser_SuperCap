#include "Error_Handle.h"
#include "cmsis_os.h"
#include "Cap_Main_Control.h"
#include "Cap_Buck_Control.h"
#include "Cap_Faction.h"
#include "uart_util.h"

void Task_Error_Detect(void const * argument) {
	for(ever){
		if(Cap_Control_State.Cap_Down_Tick <= HAL_GetTick() - 90)
		{
			Cap_Control_State.Cap_Down_En=0;
		}
		else
		{
			Cap_Control_State.Cap_Down_En=1;
		}
		if(Cap_Control_State.Communicate_Last_Tick <= HAL_GetTick() - 30)
		{
			Communicate_Offline_Error_Handle();
		}
		else 
		{
			Cap_Control_State.Communicate_State = Communicate_Online;
		}
		if(Cap_Control_State.Motor_Energy_Sum >= (Cap_Control_State.buff_power - 20)&& Cap_Control_State.Cap_State != 1 &&
			Cap_Control_State.Communicate_State == Communicate_Online) 
		{
				Cap_Control_State.Cap_Must_En = 1;
		}
		else if(Cap_Control_State.buff_power >= 40 && Cap_Control_State.LowPower_Push_State == LowPower_Notpush && 
			Cap_Control_State.Communicate_State == Communicate_Online) 
		{
				if(Cap_Adc_Data.Adc_Cap <= 12.0f) 
				{
					 Cap_Control_State.Cap_Must_En = 0;
				}
		}
		osDelay(1);
	}
}
uint8_t BlueTeeth_TxData[35];
void Task_BlueTooth(void const * argument) {
	for(ever)
	{
		BlueTeeth_TxData[0] = (uint8_t)0xA5;
	  
		BlueTeeth_TxData[1]  =(uint8_t)Cap_Control_State.Back_Flush_Switch_State ;
		BlueTeeth_TxData[2]  =(uint8_t)Cap_Control_State.Cap_Switch_State;
		BlueTeeth_TxData[3]  =(uint8_t)Cap_Control_State.buff_power;
		BlueTeeth_TxData[4]  =(uint8_t)Cap_Control_State.Cap_State ;
		
    BlueTeeth_TxData[5] = *((uint8_t*)(&Cap_Adc_Data.Adc_Cap));
    BlueTeeth_TxData[6] = *((uint8_t*)(&Cap_Adc_Data.Adc_Cap)+1);
    BlueTeeth_TxData[7] = *((uint8_t*)(&Cap_Adc_Data.Adc_Cap)+2);
    BlueTeeth_TxData[8] = *((uint8_t*)(&Cap_Adc_Data.Adc_Cap)+3);

    BlueTeeth_TxData[9] = *((uint8_t*)(&Cap_Adc_Data.Adc_Motor));
    BlueTeeth_TxData[10] = *((uint8_t*)(&Cap_Adc_Data.Adc_Motor)+1);
    BlueTeeth_TxData[11] = *((uint8_t*)(&Cap_Adc_Data.Adc_Motor)+2);
    BlueTeeth_TxData[12] = *((uint8_t*)(&Cap_Adc_Data.Adc_Motor)+3);
		
		BlueTeeth_TxData[13] = *((uint8_t*)(&Cap_Adc_Data.Adc_Boost));
    BlueTeeth_TxData[14] = *((uint8_t*)(&Cap_Adc_Data.Adc_Boost)+1);
    BlueTeeth_TxData[15] = *((uint8_t*)(&Cap_Adc_Data.Adc_Boost)+2);
    BlueTeeth_TxData[16] = *((uint8_t*)(&Cap_Adc_Data.Adc_Boost)+3);
		
		BlueTeeth_TxData[17] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_Motor));
    BlueTeeth_TxData[18] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_Motor)+1);
    BlueTeeth_TxData[19] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_Motor)+2);
    BlueTeeth_TxData[20] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_Motor)+3);
		
		BlueTeeth_TxData[21] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_In));
    BlueTeeth_TxData[22] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_In)+1);
    BlueTeeth_TxData[23] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_In)+2);
    BlueTeeth_TxData[24] = *((uint8_t*)(&Cap_Adc_Data.Adc_Current_In)+3);
		
		BlueTeeth_TxData[25] = *((uint8_t*)(&Cap_Control_State.ref_power));
    BlueTeeth_TxData[26] = *((uint8_t*)(&Cap_Control_State.ref_power)+1);
    BlueTeeth_TxData[27] = *((uint8_t*)(&Cap_Control_State.ref_power)+2);
    BlueTeeth_TxData[28] = *((uint8_t*)(&Cap_Control_State.ref_power)+3);
		
		BlueTeeth_TxData[29] = *((uint8_t*)(&Cap_Adc_Data.Motor_Power));
    BlueTeeth_TxData[30] = *((uint8_t*)(&Cap_Adc_Data.Motor_Power)+1);
    BlueTeeth_TxData[31] = *((uint8_t*)(&Cap_Adc_Data.Motor_Power)+2);
    BlueTeeth_TxData[32] = *((uint8_t*)(&Cap_Adc_Data.Motor_Power)+3);
		uint16_t sum = 0;
    for(int i = 1; i < 33; i++)sum+=BlueTeeth_TxData[i];
    BlueTeeth_TxData[33] = (uint8_t)sum;
    BlueTeeth_TxData[34] = (uint8_t)0x5A;
		HAL_UART_Transmit_IT(&huart2, BlueTeeth_TxData, 35);
		osDelay(100);
	}
}

void Short_Circuit_Error_Handle(void) {
	Cap_Control_Stop_Cap_PowerPath();
	Cap_Control_Stop_Out_PowerPath();
}

void Communicate_Offline_Error_Handle(void) {
	Cap_Control_State.Communicate_State = Communicate_Offline;
	Cap_Control_State.Power_Limit = 45;
	Cap_Control_State.Cap_Control_Power_Max = Cap_Control_State.Power_Limit;
}


void RxError_Handler(void){
	while(1)
	{
		osDelay(1);
	}
}
 
void Motor_Error_Handle(void){
	if( Cap_Adc_Data.Adc_Vcc >= 20.0f && Cap_Adc_Data.Adc_Motor>=27.0f){
		Cap_Control_Change_Backflush_PowerPath();
		Cap_Control_Change_Jduge_PowerPath();
	}
}

