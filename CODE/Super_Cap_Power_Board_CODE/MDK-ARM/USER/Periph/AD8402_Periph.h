#ifndef AD8402_PERIPH_H
#define AD8402_PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif 
#include"main.h"
#include"spi.h"

#define AD8402_RESET_PORT GPIOA
#define AD8402_RESET_PIN  GPIO_PIN_6


extern uint16_t AD8402_Periph_DATA[2];
extern float  AD8402_Periph_Ohm[2];

void AD8402_SetValue(float* ohm);
uint8_t AD8402_DecoderValue(float ohm);
void AD8402_RESET(void);


#endif

