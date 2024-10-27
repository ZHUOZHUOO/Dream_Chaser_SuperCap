#ifndef __CAP_MAIN_CONTROL_H
#define __CAP_MAIN_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "dac_util.h"
#include "adc_util.h"
#include "pid_alg.h" 
//#include"configue.h"

#define CAP_BUCK_SWITCH_PORT      		GPIOB
#define CAP_BUCK_SWITCH_PIN       		GPIO_PIN_9
#define CAP_KEY_PORT              		GPIOA
#define CAP_KEY_PIN               		GPIO_PIN_15

#define CAP_OUT_RES      0.004f
#define CAP_IN_RES       0.002f
#define ever ;;
#define PID_WIDE_0_13W      0 
#define PID_WIDE_14_28W		  1
#define PID_WIDE_29_W       0
#define PID


extern FunctionalState Cap_State;
extern PID_PIDTypeDef Cap_CapChargingPid;
extern PID_PIDparam_Typedef Cap_Chargingpidparam;
extern PID_PIDTypeDef Judge_BufferPowerPid;
extern PID_PIDparam_Typedef Judge_BufferPowerpidparam  ;
extern float Power_Limit;

typedef enum 
{
  Communite_Off = 0,
	Cap_Power_low = 1,
	Cap_Power_mid = 2,
	Cap_Power_Ful = 3
}Cap_Type_Show;
typedef enum {
	Back_Flush_State = 0,
	BoostSwitch_State   = 1
}Back_Flush_Switch_StateEnum;
typedef enum {
	Judge_State = 0,
	CapSwitch_State   = 1
}Cap_Switch_StateEnum;
typedef enum { 
	Cap_Enable = 1,
	Cap_Disable = 0
}Cap_Enable_StateEnum;
typedef enum { 
	Chassis_Starting = 1,
	Chassis_Not_Starting = 0
}Cap_Control_Chassis_Starting_StateEnum;
typedef enum {
	Motor_Safe = 1,
	Motor_Unsafe = 0
}Motor_Safety_StateEnum;//????????û?????
typedef enum {
	Chassis_Power_On = 1,
	Chassis_Power_Off = 0
}Chassis_Power_StateEnum;//?????????û??????
typedef enum {
	Communicate_Offline = 0,
	Communicate_Online = 1
}Communicate_StateEnum;//??????????????
typedef enum { 
	Chassis_Down = 1,
	Chassis_Not_Down = 1
}Cap_Control_Chassis_Down_StateEnum; //

typedef enum {
	 CAP_Charging_En = 1,
	 CAP_Charging_Dis = 0
}CAP_Charging_Enable_StateEnum;

typedef enum {
	 LowPower_Push = 1,
	 LowPower_Notpush= 0
}Cap_LowPower_Push_StateEnum;

typedef struct {
	float   Cap_val;
	float   Cap_Charging_Power_Ref;
	float   Boost_val;
	float   Power_Limit;
	uint8_t buff_power;
	float   ref_power ;
	uint8_t   Cap_Remain_Energy;
	float     System_sum_Power;
	uint32_t  Communicate_Last_Tick;
	uint32_t  Cap_Down_Tick;//µçÈÝ¹Ø±ÕÊ±µÄÊ±¼ä´Á
	Communicate_StateEnum Communicate_State;
	
	Back_Flush_Switch_StateEnum Back_Flush_Switch_State;
	Cap_Switch_StateEnum        Cap_Switch_State;
  uint8_t Cap_State;
	uint8_t Cap_Last_State;//¼ÇÂ¼µçÈÝµÄÉÏÒ»¸ö¿ªÆô×´Ì¬
	uint8_t Boost_State;
	uint8_t Cap_Must_En;
	uint8_t Cap_Down_En;//¼ÇÂ¼µçÈÝÓÉ¿ªÆôµ½¹Ø±Õ×´Ì¬µÄ×´Ì¬Î»
	Cap_Enable_StateEnum        Cap_Enable_State;
	Cap_Control_Chassis_Starting_StateEnum  Chassis_Starting_State;
  Cap_Control_Chassis_Down_StateEnum  Chassis_Down_State;
	uint32_t  Cap_Disable_Tick;
	uint32_t  Backflush_Tick;
	uint32_t  Cap_Enable_Tick;
	uint32_t  Motor_Heavy_Load_Tick;
	uint32_t  Motor_Light_Load_Tick;

	uint8_t   Output_En;
	Motor_Safety_StateEnum Motor_Safety_State;
	Chassis_Power_StateEnum  Chassis_Power_State;

	float Cap_Control_Power_Max;
	CAP_Charging_Enable_StateEnum Cap_Charging_State;
	float Motor_Energy_Sum;
	Cap_LowPower_Push_StateEnum LowPower_Push_State;
	uint8_t voltage_max_state;
	
	uint8_t  BackFlush_State;
	uint8_t  Cap_Switch_Down_State;
	uint32_t Cap_Switch_Down_Tick;
} Cap_Control_Typedef;


extern Cap_Control_Typedef Cap_Control_State;
extern uint8_t Uart2_Board_Community_Buffer[24];
extern uint8_t Uart3_Board_Community_Buffer[24];

void Cap_Control_Main_faction(void);
void Cap_Control_Switch_Main(Cap_Switch_StateEnum Cap_Switch_State, Back_Flush_Switch_StateEnum Back_Flush_Switch_State);

#endif

