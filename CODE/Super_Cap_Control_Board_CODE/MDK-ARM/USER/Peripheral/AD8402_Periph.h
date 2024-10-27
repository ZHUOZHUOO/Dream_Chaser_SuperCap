#ifndef AD8402_PERIPH_H
#define AD8402_PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif 
#include"main.h"
#include"spi.h"

#define RES_UP    510 
#define RES_DOWN   22

#define AD8402_RESET_PORT GPIOB
#define AD8402_RESET_PIN  GPIO_PIN_14


#define SPI_MOSI_PORT    GPIOA
#define SPI_MOSI_PIN     GPIO_PIN_7
//#define SPI_MOSI_LOW     (SPI_MOSI_PORT->BSRR = (uint32_t)SPI_MOSI_PIN << 16U)
//#define SPI_MOSI_HIGH    (SPI_MOSI_PORT->BSRR|=SPI_MOSI_PIN)

#define SPI_CS_PORT     GPIOA
#define SPI_CS_PIN      GPIO_PIN_15
//#define SPI_CS_LOW      (SPI_CS_PORT->BSRR = (uint32_t)SPI_CS_PIN << 16U)
//#define SPI_CS_HIGH     (SPI_CS_PORT->BSRR |=SPI_CS_PIN)

#define SPI_SCK_PORT    GPIOA
#define SPI_SCK_PIN     GPIO_PIN_5
//#define SPI_SCK_LOW     (SPI_SCK_PORT->BSRR = (uint32_t)SPI_SCK_PIN << 16U)
//#define SPI_SCK_HIGH    (SPI_SCK_PORT->BSRR|=SPI_SCK_PIN)

#define SPI_MOSI_LOW     HAL_GPIO_WritePin(SPI_MOSI_PORT,SPI_MOSI_PIN,GPIO_PIN_RESET)
#define SPI_MOSI_HIGH    HAL_GPIO_WritePin(SPI_MOSI_PORT,SPI_MOSI_PIN,GPIO_PIN_SET)

#define SPI_CS_LOW       HAL_GPIO_WritePin(SPI_CS_PORT,SPI_CS_PIN,GPIO_PIN_RESET)
#define SPI_CS_HIGH      HAL_GPIO_WritePin(SPI_CS_PORT,SPI_CS_PIN,GPIO_PIN_SET)

#define SPI_SCK_LOW      HAL_GPIO_WritePin(SPI_SCK_PORT,SPI_SCK_PIN,GPIO_PIN_RESET)
#define SPI_SCK_HIGH     HAL_GPIO_WritePin(SPI_SCK_PORT,SPI_SCK_PIN,GPIO_PIN_SET)

extern uint16_t AD8402_Periph_DATA[2];

extern float  AD8402_Periph_Ohm[2];

void AD8402_SetValue(float* ohm);
uint8_t AD8402_DecoderValue(float ohm);
void AD8402_RESET(void);
float AD8402_Voltage_Ref_Count(float exp_val);
//void AD8402_Send_10Bits(uint16_t spi_data);



#endif

