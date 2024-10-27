/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAP_BUCK_CONTROL_H
#define __CAP_BUCK_CONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "filter_alg.h"
#include "Queue_Str.h"
#include "dac_util.h"
#include "adc_util.h"
#include "pid_alg.h" 
/* Exported constants --------------------------------------------------------*/

typedef struct {
 float    CurrentIn_Offset;
 float    CurrentMotor_Offset;
 float    Adc_Vcc_Offset;
 float    Adc_Cap_Offset;
 float    Adc_Motor_Offset;
 WMA_filterdata Adc_Current_Filter;
 WMA_filterdata Adc_Cap_Filter;
 WMA_filterdata Adc_Vcc_Filter;
 WMA_filterdata Adc_Current_Motor_Filter;
 WMA_filterdata Adc_Motor_Filter;
 WMA_filterdata Dac_Current_Filter;
 WMA_filterdata Adc_Source_Filter;
 WMA_filterdata Adc_Boost_Filter;
 WMA_filterdata Cap_Charging_Power;
 WMA_filterdata Motor_Power_Filter;
 WMA_filterdata Adc_Vrefint_Filter;
 kalman_filterdata Adc_Current_motor_kalman_filter;
 kalman_filterdata rise_rate_motor_kalman_filter;
}Cap_ADC_Const;

typedef struct {
 float    Adc_Vcc;        //单位为V
 float    Adc_Cap; 
 float    Adc_Current_In; //单位为A
 float    Adc_Current_Motor; 
 float    Adc_Motor;
 float    Adc_Vrefint;
 float    Motor_Power;
 float    Cap_Charging_Power;
 float    True_Power;//通过Motor_Power Cap_Charging_Power在电容层面上算出ref_power真实功率
 float    Adc_Temp;
 Cap_ADC_Const Adc_Consts;
 float    Adc_current_motor_lowpass ;
 float    Adc_Source; //锟斤拷位为A
 float    Adc_Boost;
 Queue_Str_Typedef  motor_power_queue;
}Cap_ADC_Message;



extern Cap_ADC_Message Cap_Adc_Data;
extern float Dac_Now;
extern float Vref_Offset;
void ADC_GET_Main(void);
void ADC_GET_Port(void);
void Cap_Charging_Pid_Clac(void);
extern void PID_InitParam(PID_PIDparam_Typedef* ppram, float kp, float ki, float kd, float max_pid, float max_sumi);
#endif

