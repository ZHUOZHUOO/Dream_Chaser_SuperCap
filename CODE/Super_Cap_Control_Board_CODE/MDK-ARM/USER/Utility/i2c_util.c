#include "i2c_util.h"



void I2c_MasterSendMessage(I2C_HandleTypeDef* hi2c, uint16_t address, uint8_t* pData, uint16_t len) {
    if ((hi2c == NULL) || (pData == NULL)) {
        I2c_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_I2C_Master_Transmit(hi2c, address, pData, len, 1);
    if (ret != HAL_OK) {
        I2c_ErrorHandler(ret);
    }
}

void I2c_WriteSingleReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint16_t reg, uint8_t data) {
    if (hi2c == NULL) {
        I2c_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_I2C_Mem_Write(hi2c, address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1);
    if (ret != HAL_OK) {
        I2c_ErrorHandler(ret);
    }
}


void I2c_ReadSingleReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint16_t reg, uint8_t* res) {
    if ((hi2c == NULL) || (res == NULL)) {
        I2c_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_I2C_Mem_Read(hi2c, address, reg, I2C_MEMADD_SIZE_8BIT, res, 1, 1);
    if (ret != HAL_OK) {
        I2c_ErrorHandler(ret);
    }
}


void I2c_ReadMuliReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint16_t reg, uint8_t len, uint8_t* res) {
    if ((hi2c == NULL) || (res == NULL)) {
        I2c_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_I2C_Mem_Read(hi2c, address, reg, I2C_MEMADD_SIZE_8BIT, res, len, 1);
    if (ret != HAL_OK) {
        I2c_ErrorHandler(ret);
    }
}


void I2c_WriteMuliReg(I2C_HandleTypeDef* hi2c, uint16_t address, uint8_t reg, uint8_t* pData, uint8_t len) {
    if ((hi2c == NULL) || (pData == NULL)) {
        I2c_ErrorHandler(HAL_ERROR);
    }
    uint32_t ret = HAL_I2C_Mem_Write(hi2c, address, reg, I2C_MEMADD_SIZE_8BIT, pData, len, 1);
    if (ret != HAL_OK) {
        I2c_ErrorHandler(ret);
    }
}

void I2c_ErrorHandler(uint32_t ret) {
    while (1) {
        return;
    }
}

