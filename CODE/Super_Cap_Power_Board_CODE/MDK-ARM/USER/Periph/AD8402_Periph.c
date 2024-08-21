#include"AD8402_Periph.h"

#include"cmsis_os.h"
#define AD8402_TYPE 100.0f


uint16_t AD8402_Periph_DATA[2] = {0 , 0};

void AD8402_SetValue(float* ohm) {
	
	uint8_t value[2];
	value[0] = AD8402_DecoderValue(ohm[0]);
	value[1] = AD8402_DecoderValue(ohm[1]);

	AD8402_Periph_DATA[0] = 0x00 << 8 | value[0] ;
	AD8402_Periph_DATA[1] = 0x01 << 8 | value[1] ;
	HAL_SPI_Transmit(&hspi1, (uint8_t*)AD8402_Periph_DATA, 2, 100);
	
}

uint8_t AD8402_DecoderValue(float ohm) {
	
	uint8_t value;
//	if(ohm>=9.99f) {
//		value = 255;
//	}
//	else {
//		value = (uint8_t) ohm*255.0/AD8402_TYPE ;
//	}
	
	value =  ohm*255.0/AD8402_TYPE  ;

	if(value >= 255||(ohm*255.0/AD8402_TYPE )>=254.5) value = 255;
	
	return value;
}

void AD8402_RESET(void) {

	HAL_GPIO_WritePin(AD8402_RESET_PORT , AD8402_RESET_PIN , GPIO_PIN_RESET);
	osDelay(1);
	HAL_GPIO_WritePin(AD8402_RESET_PORT , AD8402_RESET_PIN , GPIO_PIN_SET) ;
}





