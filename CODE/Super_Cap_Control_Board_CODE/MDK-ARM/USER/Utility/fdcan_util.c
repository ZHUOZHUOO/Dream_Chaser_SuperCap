#include "fdcan_util.h"
#include "core_cm4.h"
#include "Cap_Communite.h"

void FDCAN_InitTxHander(FDCAN_TxHeaderTypeDef* pheader, uint32_t id, uint32_t dlc,uint32_t baudrateswitch,uint32_t can_type){
    
    pheader->Identifier = id;
		if(id>=0x800) {
		pheader->IdType = FDCAN_EXTENDED_ID;
		}
		else {
		pheader->IdType =   FDCAN_STANDARD_ID;
		}
    pheader->TxFrameType =  FDCAN_DATA_FRAME;
    pheader->DataLength = dlc;
    pheader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    pheader->BitRateSwitch =  baudrateswitch;
    pheader->FDFormat = can_type;
    pheader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    pheader->MessageMarker = 0;
  
}
void FDCAN_IntFilterAndStart(FDCAN_HandleTypeDef *phfdcan) {
	
  FDCAN_FilterTypeDef sFilterConfig;
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK ;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x0400;
  sFilterConfig.FilterID2 = 0x0000;
  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
    FDCAN_ErrorHandler();
  }
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    FDCAN_ErrorHandler();
  }
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
    FDCAN_ErrorHandler();
  }

  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    FDCAN_ErrorHandler();
  }
    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_BUS_OFF, 0) != HAL_OK) { 
		FDCAN_ErrorHandler(); 
		}

 
}
FDCAN_TxHeaderTypeDef ptxhead;
void FDCAN_SendMessageWithBaudSwitch(FDCAN_HandleTypeDef *hfdcan,uint8_t* pdata,uint32_t dlc,uint32_t id) {
	
	FDCAN_TxHeaderTypeDef txhead;
	FDCAN_InitTxHander(&txhead,id,dlc,FDCAN_BRS_ON,FDCAN_FD_CAN);
	ptxhead = txhead;
	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txhead, pdata);
	
}
	

	FDCAN_TxHeaderTypeDef txhead;
void FDCAN_SendMessageWithOutBaudSwitch(FDCAN_HandleTypeDef *hfdcan,uint8_t* pdata,uint32_t dlc,uint32_t id) {
	

	FDCAN_InitTxHander(&txhead,id,dlc,FDCAN_BRS_OFF,FDCAN_CLASSIC_CAN );
	ptxhead = txhead;
	if(HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txhead, pdata)!=HAL_OK) {
		FDCAN_ErrorHandler();
	}
	
}


uint8_t RxData[64];FDCAN_RxHeaderTypeDef RxHeader;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE)!=RESET) {
        if(hfdcan->Instance == FDCAN1) {
            
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData);
            switch(RxHeader.Identifier) {
                    case 0x98:
										     Cap_Communicator_toBoss(RxData);break;
                default    : break;
                }
						if( HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0)!= HAL_OK) {
							FDCAN_ErrorHandler();
					}
        }
    }
}


void FDCAN_ErrorHandler(void)
{
	
   while(1) {
		 
//		 	__set_FAULTMASK(1);
//			HAL_NVIC_SystemReset();
		 return;
       
   }
}
