#ifndef __CAP_FACTION_H
#define __CAP_FACTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "Cap_Main_Control.h"
	
	
#define CAP_POWERPATH_PORT        		GPIOC
#define CAP_POWERPATH_PIN         		GPIO_PIN_15
#define JDUGE_POWERPATH_PORT      		GPIOC
#define JDUGE_POWERPATH_PIN       		GPIO_PIN_14

#define BOOST_POWERPATH_PORT     		    GPIOB
#define BOOST_POWERPATH_PIN       		  GPIO_PIN_5
#define BACKFLUSH_POWERPATH_PORT        GPIOB
#define BACKFLUSH_POWERPATH_PIN         GPIO_PIN_6

void Cap_Control_Stop_Cap_PowerPath(void) ;
void Cap_Control_Stop_Out_PowerPath(void) ;
void Cap_Control_Change_Cap_PowerPath(void);
void Cap_Control_Change_Jduge_PowerPath(void);
void Cap_Control_Change_Boost_PowerPath(void);
void Cap_Control_Change_Backflush_PowerPath(void);
void Soft_Start(void);
void Cap_Control_SetCurrent(float current_val);
void Cap_Control_SetVoltage(float vol);
void CAP_Charging_Disable(void);
void CAP_Charging_Enable(void);
void Cap_Adc_Init(void);
void ADC_Vrefint_Init(void);
float Buck_Voltage_Decoder(float exp_val);
#endif



