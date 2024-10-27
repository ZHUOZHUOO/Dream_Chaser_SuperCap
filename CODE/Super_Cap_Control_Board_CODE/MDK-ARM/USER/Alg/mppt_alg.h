#ifndef MTTP_ALG_H
#define MTTP_ALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
 #define float32 float
 #define int16 int16_t
 
//*********** Structure Definition ********//
typedef struct {
	float32  Ipv;
	float32  Vpv;
	float32  DeltaPmin;
	float32  MaxVolt;
	float32  MinVolt;
	float32  Stepsize;
	float32  VmppOut;
	float32  DeltaP;
	float32  PanelPower;
	float32  PanelPower_Prev;
	int16 mppt_enable;
	int16 mppt_first;
} mppt_incc_typedef;
extern mppt_incc_typedef mppt_incc_handle;
void MPPT_PNO_F_FUNC(mppt_incc_typedef *v);
void MPPT_PNO_F_init(mppt_incc_typedef *v);
 #endif
