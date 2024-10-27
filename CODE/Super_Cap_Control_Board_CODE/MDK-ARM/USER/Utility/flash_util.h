
 
#ifndef __FLASH__UTIL_H
#define __FLASH__UTIL_H
 

#include "main.h"

#define  Current_OffSet_Address   0x08009cb0 
#define  FLASH_WriteAddress       0x08009cb0         	    //flash 保存数据地址写在靠后位置，防止破坏主程序
#define  FLASH_ReadAddress        FLASH_WriteAddress		  //两个地址间长度为4，所以一页只能写1024/4 = 256个数据
 
#define FLASH_BUFFSIZE 10			//FLASH buff length, length_max = 256
 


uint32_t Flash_Get_Page(uint32_t Addr);
uint32_t Flash_Get_Bank(uint32_t Addr);
void Flash_ErrorHandle(void);
void Flash_Erase(uint32_t Add);
void Flash_Write_Uint8_Array(uint8_t *src, uint32_t dest_addr, uint32_t Len);
void Flash_Write_Float(float value,uint32_t dest_addr);
void Flash_Read_Float(float *value,uint32_t dest_addr);
 
 
 
 
 
 
 
#endif 

