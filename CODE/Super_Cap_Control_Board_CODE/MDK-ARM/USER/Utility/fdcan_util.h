
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FDCAN_UTIL_H
#define __FDCAN_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif
#include"fdcan.h"

void FDCAN_ErrorHandler(void);
void FDCAN_InitTxHander(FDCAN_TxHeaderTypeDef* pheader, uint32_t id, uint32_t dlc,uint32_t baudrateswitch,uint32_t can_type);
void FDCAN_IntFilterAndStart(FDCAN_HandleTypeDef *phfdcan) ;
void FDCAN_SendMessageWithBaudSwitch(FDCAN_HandleTypeDef *hfdcan,uint8_t* pdata,uint32_t dlc,uint32_t id);

void FDCAN_SendMessageWithOutBaudSwitch(FDCAN_HandleTypeDef *hfdcan,uint8_t* pdata,uint32_t dlc,uint32_t id);

#endif
