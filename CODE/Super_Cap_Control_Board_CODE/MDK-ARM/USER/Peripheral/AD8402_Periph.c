#include"AD8402_Periph.h"
#include"cmsis_os.h"


uint16_t AD8402_Periph_DATA[2] = {0 , 0};
float  AD8402_Periph_Ohm[2];


void AD8402_SetValue(float* ohm) {
	
	uint8_t value[2];
	value[0] = AD8402_DecoderValue(ohm[0]);
	value[1] = AD8402_DecoderValue(ohm[1]);

	AD8402_Periph_DATA[0] = 0x00 << 8 | value[0] ;
	AD8402_Periph_DATA[1] = 0x01 << 8 | value[1] ;
	HAL_SPI_Transmit(&hspi2, (uint8_t*)AD8402_Periph_DATA, 2, 100);
	
}

uint8_t AD8402_DecoderValue(float ohm) {
	
	uint16_t value;
	value = (uint16_t) (ohm*255.0f/100.0f) ;
	if(value >= 255) value = 255;
	return (uint8_t)value;
}

void AD8402_RESET(void) {

	HAL_GPIO_WritePin(AD8402_RESET_PORT , AD8402_RESET_PIN , GPIO_PIN_RESET);
	osDelay(1);
	HAL_GPIO_WritePin(AD8402_RESET_PORT , AD8402_RESET_PIN , GPIO_PIN_SET) ;
}

float AD8402_Voltage_Ref_Count(float exp_val)
{
		if(exp_val < 6.5f) exp_val = 6.5f;
  	float AD8402_Value = 0;
  	AD8402_Value = (1.2f* (RES_DOWN + RES_UP) - exp_val * RES_DOWN) / (exp_val-1.20f) ;
  	return AD8402_Value;
}









