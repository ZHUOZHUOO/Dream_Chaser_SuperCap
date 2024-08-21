#include "spi_util.h"



extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;


/**
  * @brief          Initnation SPI
  * @param          hspi: The spi handle
  * @retval         NULL
  */
void Spi_Init(SPI_HandleTypeDef *hspi) {
    hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    uint32_t ret;

    if (HAL_SPI_Init(hspi) != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
}
#if(0)

/**
  * @brief          Initnation SPI for DMA receive data
  * @param          hspi: The spi handle
  * @param          tx_buff: Transmit data buff
  * @param          rx_buff: Recive data buff
  * @param          num: The data length
  * @retval         NULL
  */
void Spi_DMAInit(SPI_HandleTypeDef *hspi, uint32_t tx_buf, uint32_t rx_buf, uint16_t num) {
    SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);
    SET_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);


    __HAL_SPI_ENABLE(hspi);

    // disable DMA
    __HAL_DMA_DISABLE(&hdma_spi1_rx);
    while(hdma_spi1_rx.Instance->CCR & DMA_SxCR_EN) {
        __HAL_DMA_DISABLE(&hdma_spi1_rx);
    }
    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_rx, DMA_LISR_TCIF2);
    hdma_spi1_rx.Instance->CPAR = (uint32_t) & (SPI1->DR);

    // memory buffer
    //hdma_spi1_rx.Instance->M0AR = (uint32_t)(rx_buf);
		hdma_spi1_rx.Instance ->CMAR = (uint32_t)(rx_buf);

    // data length
    //__HAL_DMA_SET_COUNTER(&hdma_spi1_rx, num);
		hdma_spi1_rx.Instance->CNDTR = num;
    __HAL_DMA_ENABLE_IT(&hdma_spi1_rx, DMA_IT_TC);

    // disable DMA
    __HAL_DMA_DISABLE(&hdma_spi1_tx);
    while(hdma_spi1_tx.Instance->CCR & DMA_SxCR_EN) {
        __HAL_DMA_DISABLE(&hdma_spi1_tx);
    }
    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_LISR_TCIF3);
    hdma_spi1_tx.Instance->CMAR = (uint32_t) & (SPI1->DR);

    // memory buffer
    hdma_spi1_tx.Instance->CMAR = (uint32_t)(tx_buf);
    // data length
		
    //__HAL_DMA_SET_COUNTER(&hdma_spi1_tx, num);
		hdma_spi1_tx.Instance->CNDTR = num ;
}


/**
  * @brief          SPI for DMA receive data
  * @param          hspi: The spi handle
  * @param          tx_buff: Transmit data buff
  * @param          rx_buff: Recive data buff
  * @param          ndtr: The data length
  * @retval         NULL
  */
void Spi_DMAEnable(SPI_HandleTypeDef *hspi, uint32_t tx_buff, uint32_t rx_buff, uint16_t ndtr) {
    // disable DMA

    __HAL_DMA_DISABLE(&hdma_spi1_rx);
    __HAL_DMA_DISABLE(&hdma_spi1_tx);
    while(hdma_spi1_rx.Instance->CR & DMA_SxCR_EN) {
        __HAL_DMA_DISABLE(&hdma_spi1_rx);
    }
    while(hdma_spi1_tx.Instance->CR & DMA_SxCR_EN) {
        __HAL_DMA_DISABLE(&hdma_spi1_tx);
    }
    // clear flag
    __HAL_DMA_CLEAR_FLAG (hspi->hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi->hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmarx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi->hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmarx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi->hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmarx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi->hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmarx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi->hdmarx));

    __HAL_DMA_CLEAR_FLAG (hspi->hdmatx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi->hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmatx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi->hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmatx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi->hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmatx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi->hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi->hdmatx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi->hdmatx));
    // set memory address
    hdma_spi1_rx.Instance->M0AR = rx_buff;
    hdma_spi1_tx.Instance->M0AR = tx_buff;
    // set data length
    __HAL_DMA_SET_COUNTER(&hdma_spi1_rx, ndtr);
    __HAL_DMA_SET_COUNTER(&hdma_spi1_tx, ndtr);
    // enable DMA
    __HAL_DMA_ENABLE(&hdma_spi1_rx);
    __HAL_DMA_ENABLE(&hdma_spi1_tx);
}

#endif
/**
  * @brief          Receive data or command to spi address(For DMA)
  * @param          hspi: The spi handle
  * @param          pData: To be received data
  * @param          len: The data length
  * @retval         NULL
  */
void Spi_ReceiveDataDMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len) {

    if ((hspi == NULL) || (pData == NULL)) {
        Spi_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_SPI_Receive_DMA(hspi, pData, len);
    if (ret != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
    
}


/**
  * @brief          Receive data or command to spi address
  * @param          hspi: The spi handle
  * @param          pData: To be received data
  * @param          len: The data length
  * @retval         NULL
  */
void Spi_ReceiveData(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len) {

    if ((hspi == NULL) || (pData == NULL)) {
        Spi_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_SPI_Receive(hspi, pData, len, 10);
    if (ret != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
    
}


/**
  * @brief          Send data or command to spi address(For DMA)
  * @param          hspi: The spi handle
  * @param          pData: To be transmit data
  * @param          len: The data length
  * @retval         NULL
  */
void Spi_TransmitDataDMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len) {

    if ((hspi == NULL) || (pData == NULL)) {
        Spi_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_SPI_Transmit_DMA(hspi, pData, len);
    if (ret != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
    
}


/**
  * @brief          Send data or command to spi address
  * @param          hspi: The spi handle
  * @param          pData: To be transmit data
  * @param          len: The data length
  * @retval         NULL
  */
void Spi_TransmitData(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len) {

    if ((hspi == NULL) || (pData == NULL)) {
        Spi_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_SPI_Transmit(hspi, pData, len, 10);
    if (ret != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
    
}


/**
  * @brief          Swap a data or command to spi address
  * @param          hspi: The spi handle
  * @param          pData: To be transmit data
  * @param          len: The data length
  * @retval         NULL
  */
uint8_t Spi_SwapAbyteData(SPI_HandleTypeDef *hspi, uint8_t txdata) {
    
    uint8_t rx_data;
    uint32_t ret = HAL_SPI_TransmitReceive(hspi, &txdata, &rx_data, 1, 100000);
    if (ret != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
    return rx_data;
    
}


/**
  * @brief          Swap muli data or command to spi address
  * @param          hspi: The spi handle
  * @param          pData: To be transmit data
  * @param          len: The data length
  * @retval         NULL
  */
void Spi_ReadMuliReg(SPI_HandleTypeDef *hspi, uint8_t *rx_data, uint8_t len) {
    
    while (len != 0) { 
        *rx_data = Spi_SwapAbyteData(hspi, 0x55);
        rx_data++;
        len--;
    }
}


/**
  * @brief          Swap data or command to spi address(For DMA)
  * @param          hspi: The spi handle
  * @param          pData: To be transmit data
  * @param          len: The data length
  * @retval         NULL
  */
void Spi_SwapDataDMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t len) {

    if ((hspi == NULL) || (pTxData == NULL) || (pRxData == NULL)) {
        Spi_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_SPI_TransmitReceive_DMA(hspi, pTxData, pRxData, len);
    if (ret != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
    
}


/**
  * @brief          Swap data or command to spi address
  * @param          hspi: The spi handle
  * @param          pData: To be transmit data
  * @param          len: The data length
  * @retval         NULL
  */
void Spi_SwapData(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t len) {

    if ((hspi == NULL) || (pTxData == NULL) || (pRxData == NULL)) {
        Spi_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_SPI_TransmitReceive(hspi, pTxData, pRxData, len, 10);
    if (ret != HAL_OK) {
        Spi_ErrorHandler(ret);
    }
    
}


/**
  * @brief      Spi error handler
  * @param      ret: error data
  * @retval     NULL
  */
void Spi_ErrorHandler(uint32_t ret) {
    while (1) {
        return;
    }
}

