
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_UTIL_H
#define __CAN_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include"can.h"

void Can_IntFilterAndStart(CAN_HandleTypeDef *phcan) ;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *phcan) ;
void Can_RxMessageCallback(CAN_HandleTypeDef *phcan) ;
void Can_ErrorHandler(uint32_t ret) ;
void Can_InitTxHander(CAN_TxHeaderTypeDef* pheader,uint32_t idtype, uint32_t stdid, uint32_t dlc, uint32_t extid);

#endif
