/**
 * BattleSpirit Framework Header File
 *
 * File:        crc_algo.h
 * Brief:       本文件包含CRC校验的相关算法函数
 * Author:      Chen Kangbing
 * Modified:    2019/12/21 16:25:24
 *
 */
#ifndef CRC_ALGO_H
#define CRC_ALGO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"


extern unsigned char CRC8;
extern uint16_t CRC16;
extern uint16_t CRC16_INIT;
extern unsigned char CRC8_INIT;


unsigned char CRC_GetCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength, char ucCRC8);
unsigned int CRC_VerifyCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength);
void CRC_AppendCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t CRC_GetCRC16CheckSum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
unsigned int CRC_VerifyCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);
void CRC_AppendCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);


#ifdef __cplusplus
}
#endif

#endif
