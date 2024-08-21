
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_UTIL_H
#define __UART_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "usart.h"
/* Exported constants --------------------------------------------------------*/

    /* Exported functions prototypes----------------------------*/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *uartHandle);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Uart_ReceiveDMA(UART_HandleTypeDef *huart, uint8_t rxdata[], uint32_t size);
void Uart_ReceiveHandler(UART_HandleTypeDef *huart);
void Uart_DMA_Init(UART_HandleTypeDef *huart);
void Remote_DataHandle(uint8_t rx[]);
void Uart_ErrorHandler(uint32_t ret);
void Uart_SendMessage(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout);
uint16_t Uart_DMACurrentDataCounter(DMA_Channel_TypeDef *dma_stream) ;
//uint16_t Uart_DMACurrentDataCounter(DMA_Stream_TypeDef *dma_stream);


#endif
