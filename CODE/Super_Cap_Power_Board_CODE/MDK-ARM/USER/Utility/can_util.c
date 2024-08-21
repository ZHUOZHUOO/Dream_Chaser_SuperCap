#include "can_util.h"
#include "Boost_Control.h"



void Can_InitTxHander(CAN_TxHeaderTypeDef* pheader,uint32_t idtype, uint32_t stdid, uint32_t dlc, uint32_t extid){
    pheader->StdId = stdid;
    pheader->RTR =  CAN_RTR_DATA;
    pheader->IDE = idtype;
    if(idtype == CAN_ID_EXT) {
        pheader->ExtId = extid;
    }
    pheader->DLC = dlc;
    pheader->TransmitGlobalTime = DISABLE;
}

void Can_IntFilterAndStart(CAN_HandleTypeDef *phcan) {

    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    while (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK) ;
    while (HAL_CAN_Start(&hcan) != HAL_OK) ;
    while (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) ;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *phcan) {
    
    Can_RxMessageCallback(phcan);
}

void Can_RxMessageCallback(CAN_HandleTypeDef *phcan) {
  
    Boost_Control_Decoder(&hcan);
}
void Can_ErrorHandler(uint32_t ret)
{
    while(1) {
        return ;
    }
}

