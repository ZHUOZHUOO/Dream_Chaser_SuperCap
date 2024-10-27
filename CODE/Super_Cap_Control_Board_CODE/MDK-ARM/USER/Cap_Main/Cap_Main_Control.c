#include "Cap_Main_Control.h"
#include "Cap_Buck_Control.h"
#include "Cap_Faction.h"
#include "boost_Periph.h"
#include "AD8402_Periph.h"
#include "cmsis_os.h"
#include "core_cm4.h"
#include "fdcan_util.h"
#include "uart_util.h"
#include "math.h"
#include "tim.h"
#include "configue.h"
#include "iwdg.h"

Cap_Control_Typedef Cap_Control_State = { 26, 40, 24, 40, 50, 45, 0, 0, 0, 0,
	                                        Communicate_Offline,Back_Flush_State,Judge_State,0,0,0,0,0,Cap_Enable,Chassis_Not_Starting,Chassis_Not_Down,
                                          0,0,0,0,0,0,Motor_Unsafe,Chassis_Power_On,0,CAP_Charging_En,0,LowPower_Notpush,0,0,0};

//float    backflush_current = -0.8f;
//float    backflush_voltage = 15.0f;
  PID_PIDparam_Typedef Cap_Chargingpidparam = {0.003f, 0.00f, 0.0002f, 1, 10000}; 
  PID_PIDparam_Typedef Judge_BufferPowerpidparam = {0.1f, 0.2f, 2, 1000};//正常功率环

  PID_PIDTypeDef Cap_CapChargingPid;
  PID_PIDTypeDef Judge_BufferPowerPid;

uint8_t Restart_flag;
uint8_t Last_Restart_flag;

void Task_Main_Control(void const * argument)
{
  osDelay(10);
  Cap_Adc_Init();
	Cap_Control_State.Cap_val = Cap_Charge_Vol;

	Cap_Control_State.Motor_Safety_State = Motor_Safe;
	Cap_Control_State.Chassis_Power_State = Chassis_Power_On;
	
	Cap_Control_State.Cap_Control_Power_Max = Cap_Control_State.Power_Limit;
	Cap_Control_Change_Cap_PowerPath();
	
	AD8402_RESET();	
	Soft_Start();

  CAP_Charging_Disable();
	Adc_Init();
	ADC_Vrefint_Init();//IntVref较准,
	DAC_Init();	
	DAC_SetVal(0.02f);

	Cap_Control_SetVoltage(Cap_Control_State.Cap_val);
	
  HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start(&htim2);
	Restart_flag=0;
	
  for(ever)
  {
		if(Cap_Adc_Data.Adc_Vcc<=12.0f)//检测裁判系统断电
		{
				Cap_Control_Change_Cap_PowerPath();
				Cap_Control_Stop_Out_PowerPath();
				CAP_Charging_Disable();
				Cap_Control_State.Boost_State = 0;
			  Dac_Now = 0.02f;
				osDelay(1);
			  Restart_flag=1;
			  IWDG_State.Task_Main_Control_Down = 1;
				continue;
		}
		else if(Last_Restart_flag==1 && Cap_Adc_Data.Adc_Vcc > 12.0f)//复活重启
		{
			Last_Restart_flag = 0;
			Restart_flag = 0;
			Soft_Start();
			IWDG_State.Task_Main_Control_Down = 1;
			continue;
		}
		
		  Cap_Control_SetVoltage(Cap_Control_State.Cap_val);//电容充电26.8V
	    CAP_Charging_Enable();
			
				if(Cap_Adc_Data.Motor_Power >= ( Cap_Control_State.Power_Limit * 0.95f ) || Cap_Control_State.ref_power >= ( Cap_Control_State.Power_Limit + 5.0f )) 
			{
				Cap_Control_State.Cap_Must_En = 1;
			}
			else if(Cap_Adc_Data.Motor_Power < ( Cap_Control_State.Power_Limit * 0.85f) && Cap_Control_State.ref_power <= ( Cap_Control_State.Power_Limit - 5.0f ))
			{
				Cap_Control_State.Cap_Must_En = 0;
			}
			
		if(Cap_Adc_Data.Adc_Cap<=11.0f)
		{ 
			Cap_Control_State.Cap_Enable_State = Cap_Disable;
			if(Cap_Adc_Data.Motor_Power >= ( Cap_Control_State.Power_Limit * 0.95f ))
			{
				Cap_Control_State.Cap_Must_En = 1;
			 	Cap_Control_State.LowPower_Push_State = LowPower_Push;
			}
			else if(Cap_Adc_Data.Motor_Power < ( Cap_Control_State.Power_Limit * 0.85f)) 
			{
				Cap_Control_State.Cap_Must_En = 0;
				Cap_Control_State.LowPower_Push_State = LowPower_Notpush;
			}
			else if(Cap_Control_State.buff_power >= 30)
			{
				Cap_Control_State.Cap_Must_En = 0;
				Cap_Control_State.LowPower_Push_State = LowPower_Notpush;
			}
			if(Cap_Control_State.buff_power <= 20.0f || (Cap_Control_State.buff_power <= 40 &&Cap_Adc_Data.Motor_Power >= Cap_Control_State.Power_Limit + 20.0f)) 
			{
				Cap_Control_State.Cap_Must_En = 1;
				Cap_Control_State.LowPower_Push_State = LowPower_Push;
			}
		}
		else if(Cap_Adc_Data.Adc_Cap>=12.0f )
		{
			Cap_Control_State.Cap_Enable_State = Cap_Enable;
		}

		Cap_Control_Main_faction();
	
		Last_Restart_flag = Restart_flag;
		Restart_flag = 0;
	
		IWDG_State.Task_Main_Control_Down = 1;
		osDelay(1);
  }
}

void Cap_Control_Main_faction(void)
{
		uint8_t Last_Cap_Switch_State_Temp = Cap_Control_State.Cap_Switch_State;//记录上一个电容开启状态
	
		Cap_Switch_StateEnum 				 Cap_Switch_State_Temp 				= Cap_Control_State.Cap_Switch_State;
	  Back_Flush_Switch_StateEnum  Back_Flush_Switch_State_Temp = Cap_Control_State.Back_Flush_Switch_State;//用临时变量代替函数执行逻辑操作

	  Cap_Control_State.Cap_State = Cap_Control_State.Cap_State | Cap_Control_State.Cap_Must_En | Cap_Control_State.Cap_Down_En;//更新电容开启状态
		
		if(Cap_Control_State.Cap_Last_State == 1 && Cap_Control_State.Cap_State == 0 && Cap_Control_State.Cap_Down_En == 0 && Cap_Control_State.Cap_Down_Tick <= HAL_GetTick() - 100)
		{
			Cap_Control_State.Cap_Down_Tick = HAL_GetTick();
		}//电容开启到关闭
		
	  Cap_Control_State.Cap_State = Cap_Control_State.Cap_State | Cap_Control_State.Cap_Must_En | Cap_Control_State.Cap_Down_En;//更新电容开启状态
		
		Back_Flush_Switch_State_Temp = BoostSwitch_State;//因为二路1473被拆，先随便给一个赋值
		
    if(Cap_Adc_Data.Adc_Motor>=30.0f)//反冲
		{
			Back_Flush_Switch_State_Temp  = Back_Flush_State; 
			Cap_Switch_State_Temp 				= CapSwitch_State;
			Cap_Control_State.voltage_max_state = 1;
			Cap_Control_State.Boost_State = 1;
			Boost_BoostSetValue(&Boost_BoostDataHandler);
			Cap_Control_Switch_Main(Cap_Switch_State_Temp,Back_Flush_Switch_State_Temp);
			return;
		}
		else 
    { Cap_Control_State.voltage_max_state = 0;}
		
		if((Cap_Control_State.Cap_State == 1 && Cap_Adc_Data.Adc_Cap >12.0f)|| Cap_Control_State.Cap_Must_En == 1)//需要开电容//电容大于12V就开电容
			{
			   if(Cap_Adc_Data.Adc_Cap >= 22.0f)
		     {
				   Cap_Switch_State_Temp = CapSwitch_State; 
					 Cap_Control_State.Boost_State = 1;
			   }
			   else if(Cap_Adc_Data.Adc_Cap < 22.0f)
			   {  
   				 Cap_Control_State.Boost_State = 1;
					 uint32_t temp_tick = HAL_GetTick();
					 if(	temp_tick	- Cap_Control_State.Motor_Light_Load_Tick <=20 &&  Cap_Control_State.Motor_Light_Load_Tick<=Cap_Control_State.Motor_Heavy_Load_Tick )
					 {
						 Cap_Switch_State_Temp = Judge_State;
				   }
					 else 
					 {
						 Cap_Switch_State_Temp = CapSwitch_State;
					 }
			   }
			}
			else if(Cap_Adc_Data.Adc_Cap>12.0f&&Cap_Control_State.Cap_Enable_State == Cap_Disable)//大于12v将电容设为可开启状态
			{
				Cap_Control_State.Cap_Enable_State = Cap_Enable;
			}
			else //除此之外不开电容时,裁判系统直接供电
			{ 
				Cap_Switch_State_Temp = Judge_State;
				Cap_Control_State.Boost_State = 1;
			}

			
			Cap_Control_State.Cap_Last_State = Cap_Control_State.Cap_State;
			
		if(Cap_Adc_Data.Adc_Cap<22.0f)
		{
				uint32_t temp_tick = HAL_GetTick();
				if(((Last_Cap_Switch_State_Temp == 1 && Cap_Switch_State_Temp == Judge_State)|| \
						(temp_tick	- Cap_Control_State.Motor_Light_Load_Tick <=20 &&  \
						 Cap_Control_State.Motor_Light_Load_Tick<=Cap_Control_State.Motor_Heavy_Load_Tick && \
						 Cap_Adc_Data.Adc_Cap < 14.0f)) &&\
					   Cap_Control_State.Cap_Switch_Down_Tick <= HAL_GetTick() - 220)
				{
					Cap_Control_State.Cap_Switch_Down_Tick = HAL_GetTick();
					Cap_Switch_State_Temp = Judge_State;
					Back_Flush_Switch_State_Temp = Back_Flush_State;
					Cap_Control_State.Cap_Switch_Down_State = 1;
				}
				else if(Cap_Control_State.Cap_Switch_Down_State == 1 && Cap_Control_State.Cap_Switch_Down_Tick >= HAL_GetTick() - 200)
				{
					Cap_Switch_State_Temp = Judge_State;
					Back_Flush_Switch_State_Temp = Back_Flush_State;
				}
				else
				{
					Cap_Control_State.Cap_Switch_Down_State = 0;
				}
		}
		
		Boost_BoostSetValue(&Boost_BoostDataHandler);//设置boost电压，由电容供电
		Cap_Control_Switch_Main(Cap_Switch_State_Temp,Back_Flush_Switch_State_Temp);
		return;
	}


void Cap_Control_Switch_Main(Cap_Switch_StateEnum Cap_Switch_State, Back_Flush_Switch_StateEnum Back_Flush_Switch_State)//通道切换执行函数
{
	if(Cap_Switch_State == CapSwitch_State){Cap_Control_Change_Cap_PowerPath();}
	else {Cap_Control_Change_Jduge_PowerPath();}
	
	if(Back_Flush_Switch_State == BoostSwitch_State){Cap_Control_Change_Boost_PowerPath();}
	else {Cap_Control_Change_Backflush_PowerPath();}
}
