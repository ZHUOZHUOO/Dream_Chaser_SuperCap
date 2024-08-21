/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOST_CONTROL_H
#define __BOOST_CONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

#define ever       ;;
#define SELF_ID    0xE1
#define RES_UP     510
#define RES_DOWN   22

#include"can_util.h"
#include"uart_util.h"

extern float Boost_AD8402_Value[2];




typedef enum  { 
	
	BOOST_MODE_DCM = 1,
	BOOST_MODE_CCM = 2
}Boost_Mode;

typedef enum  {
	
	BOOST_WORK_ENABLE = 1,
	BOOST_WORK_DISABLE = 0
}Boost_Work_Sat;

typedef struct {
	
	Boost_Work_Sat sat;
    Boost_Mode     mode;
    uint8_t        rx_id;
    uint8_t        tx_id;
    float          voltage;

	
}Boost_CtrTypedef;

typedef struct 
	{
		float lowpass_param;
		float last_value;
		float value;
	} lowpass_filterdata;


void BoostControlTask(void const * argument);
void LedShineTask(void const * argument);
void AdcOutTask(void const * argument);
//void Can_SendMessage (uint8_t target_id, float voltage, Boost_Mode mode , Boost_Work_Sat well);
void Can_SendMessage (void);
void Boost_Control_Decoder( CAN_HandleTypeDef *phcan);
void Boost_CommunicatorDecoder(uint8_t* rxdata);
void  RxError_Handler(void);
float lowpass_filter(lowpass_filterdata * pdata ,float pvalue);
float Boost_Voltage_Decoder(float exp_val);
void Cap_Control_Soft_Start(void);
void Boost_Control_Stop_Output(void) ;
void Boost_Control_Start_Output(void);
extern Boost_CtrTypedef Boost_BoostCtrState ;



#endif
