#ifndef __BOOST_PERIPH_H
#define __BOOST_PERIPH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include"fdcan.h"
#include"fdcan_util.h"

typedef struct{
    float voltage ; 
		FDCAN_HandleTypeDef *phcan;
    uint32_t Can_SendID;
} Boost_Boostdata;

extern Boost_Boostdata Boost_BoostDataHandler;
void Boost_BoostSetValue(Boost_Boostdata * pdata);

#endif
