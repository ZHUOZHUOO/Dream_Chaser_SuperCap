#include "Cap_Buck_Control.h"
#include "Cap_Main_Control.h"
#include "Cap_Faction.h"
#include "configue.h"
#include "adc_util.h"
#include "cmsis_os.h"
#include "iwdg.h"

Cap_ADC_Message Cap_Adc_Data;



void Task_Buck_Control(void const * argument)
{
  /* USER CODE BEGIN Task_Buck_Control */
  /* Infinite loop */

	PID_Init(&Judge_BufferPowerPid);//整车功率环，根据主控的缓冲功率计算电容充电功率上限
	PID_Init(&Cap_CapChargingPid);
	PID_SetPIDRef(&Cap_CapChargingPid,10);
	for(ever)
		{
		   if(Cap_Control_State.buff_power <= 28.0f) //低缓冲功率必须开电容
	   	{
			   Cap_Control_State.Cap_Must_En = 1;
		  }
		   else if(Cap_Control_State.buff_power >= 40.0f && Cap_Control_State.buff_power!=253.0f)
			{
			   Cap_Control_State.Cap_Must_En = 0;
		  }

		  if(Cap_Control_State.Communicate_State == Communicate_Online && Cap_Control_State.buff_power!=253.0f && Cap_Control_State.Boost_State == 1)
		 {
			PID_SetPIDRef(&Judge_BufferPowerPid,50);
			PID_SetPIDFdb(&Judge_BufferPowerPid,Cap_Control_State.buff_power);
			PID_ClacPID(&Judge_BufferPowerPid,&Judge_BufferPowerpidparam);
			Cap_Control_State.Cap_Control_Power_Max -= PID_GetPIDOutput(&Judge_BufferPowerPid);
			if(Cap_Control_State.Cap_Control_Power_Max > Cap_Control_State.Power_Limit + energy_wide) 
			 {
				Cap_Control_State.Cap_Control_Power_Max = Cap_Control_State.Power_Limit + energy_wide;
			 }
			else if(Cap_Control_State.Cap_Control_Power_Max < Cap_Control_State.Power_Limit - energy_wide) 
			 {
				Cap_Control_State.Cap_Control_Power_Max = Cap_Control_State.Power_Limit - energy_wide;
			 }
		  }
		 else
		 {
			Judge_BufferPowerPid.output=0;
			Cap_Control_State.Cap_Control_Power_Max = Cap_Control_State.Power_Limit - 5;
		 }
		 Cap_Charging_Pid_Clac();
		 IWDG_State.Task_Buck_Control_Down = 1;
		 osDelay(20);
	 }
  /* USER CODE END Task_Buck_Control */
}

float Disable_Limit = 0.0f;
float Dac_Now = 0.1;

void Cap_Charging_Pid_Clac(void)
{
		float ref;
		if(Cap_Control_State.Cap_State == 1 ) 
		{
			ref = Cap_Control_State.Cap_Control_Power_Max;//期望值
		}
		else {
			float temp_motor_power = Cap_Adc_Data.Motor_Power>=0?Cap_Adc_Data.Motor_Power:0;
			ref =   Cap_Control_State.Cap_Control_Power_Max - temp_motor_power;
		}
		Cap_Control_State.Cap_Charging_Power_Ref  = (ref>= Disable_Limit?ref: Disable_Limit);
		if(Cap_Control_State.Cap_Charging_Power_Ref <= Disable_Limit) {
			CAP_Charging_Disable();
			return;
		}
		else 
		{
			CAP_Charging_Enable();
		}
		PID_SetPIDRef(&Cap_CapChargingPid,Cap_Control_State.Cap_Charging_Power_Ref );
		PID_SetPIDFdb(&Cap_CapChargingPid,Cap_Adc_Data.Cap_Charging_Power);
		PID_ClacPID(&Cap_CapChargingPid,&Cap_Chargingpidparam);
		
		Dac_Now = Dac_Now + Cap_CapChargingPid.output;
		
		if(Dac_Now>3.0f) Dac_Now = 3.0f;
		else if(Dac_Now<0.0f) Dac_Now = 0.0f;
		Cap_Control_SetCurrent(Dac_Now);
}

/*
    follows are about ADC used in adc_util.c
*/
float    Adc_Vcc_Val[Adc_OverSampling_Times];  
float    Adc_Cap_Val[Adc_OverSampling_Times];
float    Adc_Current_In_Val[Adc_OverSampling_Times];

float    Adc_Current_Motor_Val[Adc_OverSampling_Times];
float    Adc_Motor_Val[Adc_OverSampling_Times];
float    Adc_Temp_Val[Adc_OverSampling_Times];
float    Adc_Vrefint_Val[Adc_OverSampling_Times];


float    Adc_Source_Val[Adc_OverSampling_Times];
float    Adc_Boost_Val[Adc_OverSampling_Times];

float    Vref_Offset = 1.0f;
float    Sum_Power;

float BUCK_CURRENT_IN_OFFSET 		= 1.006f;//PCB板的压降的偏执
float MOTOR_CURRENT_OUT_OFFSET  = 1.0035f;

void ADC_GET_Main(void) 
{
	for (int j = 0; j < Adc_OverSampling_Times * ADC1_CHANNEL_NUM ; j++) {
        Adc_Sum_Val[j]  = (float)Adc_Val[j] / 65535.0f * REF_V;
				      if(j%ADC1_CHANNEL_NUM == 0){Adc_Boost_Val[j/ADC1_CHANNEL_NUM] 				=  (Adc_Sum_Val[j] * Vref_Offset)*11.0f;} //cx ??? 1.248
        else  if(j%ADC1_CHANNEL_NUM == 1){Adc_Source_Val[j/ADC1_CHANNEL_NUM] 				=  (Adc_Sum_Val[j] * Vref_Offset)*11.0f;}
        else  if(j%ADC1_CHANNEL_NUM == 2){Adc_Current_Motor_Val[j/ADC1_CHANNEL_NUM] = ((Adc_Sum_Val[j] * Vref_Offset * MOTOR_CURRENT_OUT_OFFSET ) - 1.80f ) * 10.0f;}
				else  if(j%ADC1_CHANNEL_NUM == 3){Adc_Current_In_Val[j/ADC1_CHANNEL_NUM] 		= ((Adc_Sum_Val[j] * Vref_Offset * BUCK_CURRENT_IN_OFFSET   ) - 1.80f ) * 10.0f;} 
				else  if(j%ADC1_CHANNEL_NUM == 4){Adc_Temp_Val[j/ADC1_CHANNEL_NUM] 					= ((Adc_Sum_Val[j] * Vref_Offset - 0.76f) / 0.0025f + 30.0f );}
				else  if(j%ADC1_CHANNEL_NUM == 5){Adc_Vrefint_Val[j/ADC1_CHANNEL_NUM] 			=  (Adc_Sum_Val[j]);}
    }
    float temp_Temp = 0,Source_Temp = 0 ,Boost_Temp = 0,Current_In_Temp = 0,Current_Motor_Temp = 0, vrefint_Temp = 0;
    for (int j = 0; j < Adc_OverSampling_Times; j++) {
				Source_Temp         += Adc_Source_Val[j];
				Boost_Temp 					+= Adc_Boost_Val[j];
				Current_In_Temp 		+= Adc_Current_In_Val[j];
				Current_Motor_Temp  += Adc_Current_Motor_Val[j];
				temp_Temp 					+= Adc_Temp_Val[j];
			  vrefint_Temp				+= Adc_Vrefint_Val[j];
    }
  Cap_Adc_Data.Adc_Temp 			 	 					= temp_Temp / Adc_OverSampling_Times;
	Cap_Adc_Data.Adc_Boost 		 	 	 					= WMA_filter(&Cap_Adc_Data.Adc_Consts.Adc_Boost_Filter,Boost_Temp / Adc_OverSampling_Times);
	Cap_Adc_Data.Adc_Source  		 	 					= WMA_filter(&Cap_Adc_Data.Adc_Consts.Adc_Source_Filter,Source_Temp / Adc_OverSampling_Times);
  Cap_Adc_Data.Adc_Current_In 	 					= WMA_filter(&Cap_Adc_Data.Adc_Consts.Adc_Current_Filter,((Current_In_Temp/Adc_OverSampling_Times) + Cap_Adc_Data.Adc_Consts.CurrentIn_Offset));	
	Cap_Adc_Data.Cap_Charging_Power 				= WMA_filter(&Cap_Adc_Data.Adc_Consts.Cap_Charging_Power,Cap_Adc_Data.Adc_Vcc *  Cap_Adc_Data.Adc_Current_In);
	Cap_Adc_Data.Adc_current_motor_lowpass  = WMA_filter(&Cap_Adc_Data.Adc_Consts.Adc_Current_Motor_Filter,(Current_Motor_Temp/Adc_OverSampling_Times)+Cap_Adc_Data.Adc_Consts.CurrentMotor_Offset); 
	Cap_Adc_Data.Adc_Current_Motor 					= Cap_Adc_Data.Adc_current_motor_lowpass;
	Cap_Adc_Data.Adc_Vrefint			 					= vrefint_Temp;
		

	//kalman_filter(&Adc_Current_motor_kalman_filter,((Current_Motor_Temp/5.0f)+CurrentMotor_Offset));
	if(Cap_Adc_Data.Adc_Current_Motor >= 1.0f) {
		Cap_Control_State.Motor_Heavy_Load_Tick = HAL_GetTick();
	}
	else if(Cap_Adc_Data.Adc_Current_Motor<=0.2f && Cap_Adc_Data.Adc_Current_Motor>= -0.2f){
		Cap_Control_State.Motor_Light_Load_Tick = HAL_GetTick();
	}
	
	
	if(Cap_Control_State.Cap_State == 1) 
		{
			Cap_Adc_Data.True_Power = Cap_Adc_Data.Cap_Charging_Power;
		}
		else {
			float temp_motor_power = Cap_Adc_Data.Motor_Power>=0?Cap_Adc_Data.Motor_Power:0;
			Cap_Adc_Data.True_Power = Cap_Adc_Data.Cap_Charging_Power + temp_motor_power;
		}//模拟裁判系统功率
}
void ADC_GET_Port(void) 
	{
	for (int j = 0; j < Adc_OverSampling_Times * ADC2_CHANNEL_NUM; j++) {
        Adc_Sum_Val_2[j]  = (float)Adc_Val_2[j] / 65535.0f * REF_V;
				      if(j%ADC2_CHANNEL_NUM == 0) Adc_Motor_Val[j/ADC2_CHANNEL_NUM] = Adc_Sum_Val_2[j]*Vref_Offset*11.0f;
        else  if(j%ADC2_CHANNEL_NUM == 1) Adc_Cap_Val[j/ADC2_CHANNEL_NUM]   = Adc_Sum_Val_2[j]*Vref_Offset*11.0f;
				else  if(j%ADC2_CHANNEL_NUM == 2) Adc_Vcc_Val[j/ADC2_CHANNEL_NUM]   = Adc_Sum_Val_2[j]*Vref_Offset*11.0f;
    }
    float Motor_Temp = 0 ;
		float Cap_Temp = 0 ,Vcc_Temp = 0;
    for (int j = 0; j < Adc_OverSampling_Times; j++) {
        Motor_Temp      += Adc_Motor_Val[j];
				Cap_Temp        += Adc_Cap_Val[j];
				Vcc_Temp        += Adc_Vcc_Val[j];
    }
    Cap_Adc_Data.Adc_Motor = WMA_filter(&Cap_Adc_Data.Adc_Consts.Adc_Motor_Filter,Motor_Temp/Adc_OverSampling_Times*Cap_Adc_Data.Adc_Consts.Adc_Motor_Offset);        //????V
		Cap_Adc_Data.Adc_Vcc = WMA_filter(&Cap_Adc_Data.Adc_Consts.Adc_Vcc_Filter,Vcc_Temp/Adc_OverSampling_Times*Cap_Adc_Data.Adc_Consts.Adc_Vcc_Offset);        //????V
    Cap_Adc_Data.Adc_Cap = WMA_filter(&Cap_Adc_Data.Adc_Consts.Adc_Cap_Filter,Cap_Temp/Adc_OverSampling_Times*Cap_Adc_Data.Adc_Consts.Adc_Cap_Offset);

		Cap_Adc_Data.Motor_Power = WMA_filter(&Cap_Adc_Data.Adc_Consts.Motor_Power_Filter,Cap_Adc_Data.Adc_Motor*Cap_Adc_Data.Adc_Current_Motor );
}
