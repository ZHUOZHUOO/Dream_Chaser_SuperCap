#ifndef I2C_UTIL_H
#define I2C_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"

void I2c_MasterSendMessage(I2C_HandleTypeDef* hi2c, uint16_t address, uint8_t* pData, uint16_t len);
void I2c_WriteSingleReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint16_t reg, uint8_t data);
void I2c_ReadSingleReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint16_t reg, uint8_t* res);
void I2c_ReadMuliReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint16_t reg, uint8_t len, uint8_t* res);
void I2c_WriteMuliReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint8_t reg, uint8_t* pData, uint8_t len);
void I2c_ErrorHandler(uint32_t ret);


#endif



