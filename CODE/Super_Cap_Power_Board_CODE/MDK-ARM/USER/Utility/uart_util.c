#include "uart_util.h"
/* Private function -----------------------------------------------*/


void Uart_RxIdCallback(UART_HandleTypeDef *huart) {
//    if (huart == Const_Remote_UART_HANDLER) {
//            /* handle dbus data dbus_buf from DMA */
//        if ((54 - Uart_DMACurrentDataCounter(huart->hdmarx->Instance)) == 18){
//            Remote_RxCallback(Remote_rxdata);
//        }
//            /* restart dma transmission */
//        }

}

void Uart_ReceiveDMA(UART_HandleTypeDef *huart, uint8_t rxdata[], uint32_t size) {

    uint32_t tmp1 = 0;
    tmp1 = huart->RxState; 
    if (tmp1 == HAL_UART_STATE_READY) {
        if ((rxdata == NULL) || (size == 0))
        {
            return;
        }
        huart->pRxBuffPtr = rxdata;
        huart->RxXferSize = size;
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        /* Enable the DMA Stream */
        HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->RDR, (uint32_t)rxdata, size);\
        SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);   
        /* 
         * Enable the DMA transfer for the receiver request by setting the DMAR bit
         * in the UART CR3 register 
         */
    }
}

void Uart_ReceiveHandler(UART_HandleTypeDef* huart) {

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE)) {
        /* clear idle it flag avoid idle interrupt all the time */
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        /* handle received data in idle interrupt */
        /* clear DMA transfer complete flag */
        __HAL_DMA_DISABLE(huart->hdmarx);
        Uart_RxIdCallback(huart);
        
//        __HAL_DMA_SET_COUNTER(huart->hdmarx, 54);
			huart->hdmarx->Instance->CNDTR = 54 ;
        __HAL_DMA_ENABLE(huart->hdmarx);
    }
}

void Uart_DMA_Init(UART_HandleTypeDef* huart) {

    __HAL_UART_CLEAR_IDLEFLAG(huart);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
}
void Uart_SendMessage(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout) {
    /* Start the Transmission process */
    uint32_t ret = HAL_UART_Transmit(huart, txdata, size, timeout);
    if (ret != HAL_OK) {
        /* Transmission request Error */
        Uart_ErrorHandler(ret);
    }
}

void Uart_ErrorHandler(uint32_t ret) {
    while (1) {
        return;
    }
}

uint16_t Uart_DMACurrentDataCounter(DMA_Channel_TypeDef *dma_stream) {
  /* Return the number of remaining data units for DMAy Streamx */
  return ((uint16_t)(dma_stream->CNDTR));
}




