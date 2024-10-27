#include "Cap_Faction.h"
#include "Cap_Main_Control.h"
#include "Cap_Buck_Control.h"
#include "AD8402_Periph.h"
#include "cmsis_os.h"
#include "core_cm4.h"

void Cap_Control_Change_Cap_PowerPath(void)
{
	HAL_GPIO_WritePin(CAP_POWERPATH_PORT,CAP_POWERPATH_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(JDUGE_POWERPATH_PORT,JDUGE_POWERPATH_PIN,GPIO_PIN_RESET);
	Cap_Control_State.Cap_Switch_State = CapSwitch_State;
}

void Cap_Control_Change_Jduge_PowerPath(void)
{
	HAL_GPIO_WritePin(CAP_POWERPATH_PORT,CAP_POWERPATH_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(JDUGE_POWERPATH_PORT,JDUGE_POWERPATH_PIN,GPIO_PIN_SET);
	Cap_Control_State.Cap_Switch_State = Judge_State;
}

void Cap_Control_Change_Boost_PowerPath(void)
{
	HAL_GPIO_WritePin(BOOST_POWERPATH_PORT,BOOST_POWERPATH_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BACKFLUSH_POWERPATH_PORT,BACKFLUSH_POWERPATH_PIN,GPIO_PIN_RESET);
	Cap_Control_State.Back_Flush_Switch_State = BoostSwitch_State;
}

void Cap_Control_Change_Backflush_PowerPath(void)
{
	HAL_GPIO_WritePin(BOOST_POWERPATH_PORT,BOOST_POWERPATH_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BACKFLUSH_POWERPATH_PORT,BACKFLUSH_POWERPATH_PIN,GPIO_PIN_SET);
	Cap_Control_State.Back_Flush_Switch_State =  Back_Flush_State;
}

void Cap_Control_Stop_Cap_PowerPath(void)
{
	HAL_GPIO_WritePin(CAP_POWERPATH_PORT,CAP_POWERPATH_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(JDUGE_POWERPATH_PORT,JDUGE_POWERPATH_PIN,GPIO_PIN_RESET);
}

void Cap_Control_Stop_Out_PowerPath(void) 
{
	HAL_GPIO_WritePin(BOOST_POWERPATH_PORT,BOOST_POWERPATH_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BACKFLUSH_POWERPATH_PORT,BACKFLUSH_POWERPATH_PIN,GPIO_PIN_RESET);
}


void CAP_Charging_Disable(void) 
{
    HAL_GPIO_WritePin(CAP_BUCK_SWITCH_PORT,CAP_BUCK_SWITCH_PIN,GPIO_PIN_RESET);
		Cap_Control_State.Cap_Charging_State = CAP_Charging_Dis;
}

void CAP_Charging_Enable(void) 
{
    HAL_GPIO_WritePin(CAP_BUCK_SWITCH_PORT,CAP_BUCK_SWITCH_PIN,GPIO_PIN_SET);
		Cap_Control_State.Cap_Charging_State = CAP_Charging_En;
}

void Cap_Control_SetVoltage(float vol)
{

	AD8402_Periph_Ohm[1] = AD8402_Voltage_Ref_Count(vol);
	AD8402_Periph_Ohm[0] = 0.0f;
	AD8402_SetValue(AD8402_Periph_Ohm);
}

void Cap_Control_SetCurrent(float current_val)
{
	if (current_val >= 3.0f)
		current_val = 3.0f;
	else if (current_val <= 0.0f)
		current_val = 0.0f;
	DAC_SetVal(current_val);
}

void Soft_Start(void)
{
	Cap_Control_Stop_Cap_PowerPath();
	Cap_Control_Stop_Out_PowerPath();
	osDelay(20);
	Cap_Control_Change_Jduge_PowerPath();
	Cap_Control_Change_Backflush_PowerPath();
	osDelay(1500);
	Dac_Now = 0.02f;
	Cap_Control_State.Boost_State = 0;
}

void Cap_Adc_Init(void)
{
	Cap_Adc_Data.Adc_Consts.CurrentIn_Offset    = -0.07;
	Cap_Adc_Data.Adc_Consts.CurrentMotor_Offset = 0.08;
	Cap_Adc_Data.Adc_Consts.Adc_Vcc_Offset      = 1.004;
	Cap_Adc_Data.Adc_Consts.Adc_Cap_Offset      = 1.00241;
	Cap_Adc_Data.Adc_Consts.Adc_Motor_Offset    = 1.0;
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Current_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Cap_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Vcc_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Current_Motor_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Motor_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Dac_Current_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Source_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Boost_Filter);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Cap_Charging_Power);
	WMA_filter_Init(&Cap_Adc_Data.Adc_Consts.Motor_Power_Filter);
	kalman_filter_Init(&Cap_Adc_Data.Adc_Consts.Adc_Current_motor_kalman_filter,0.02,0,0,0,0.0001,100,0,0);
	kalman_filter_Init(&Cap_Adc_Data.Adc_Consts.rise_rate_motor_kalman_filter,0.02,0,0,0,0.0001,100,0,0);
}

void ADC_Vrefint_Init(void)
{
	__IO uint16_t* VREFINT_CAL = (__IO uint16_t *)(0x1FFF75AA);

	float VREFINT_CAL_DATA = 0;
	float VREFINT_CAL_VAL  = 0;
	float Vref_Offset_Sum  = 0.0f;
	
	Cap_Control_Stop_Out_PowerPath();
	osDelay(20);
	Cap_Control_Stop_Cap_PowerPath();
	osDelay(20);
	
	VREFINT_CAL_DATA = (float)*VREFINT_CAL;
	VREFINT_CAL_VAL = (VREFINT_CAL_DATA/4095.0f * 3.0f);
	for(int flag=0;flag<1000;flag++)
	{
			Vref_Offset_Sum += VREFINT_CAL_VAL/Cap_Adc_Data.Adc_Vrefint;
			osDelay(1);
	}
	Vref_Offset = Vref_Offset_Sum / 1000;
	
	Cap_Control_Change_Jduge_PowerPath();
	Cap_Control_Change_Backflush_PowerPath();
	osDelay(20);
}
