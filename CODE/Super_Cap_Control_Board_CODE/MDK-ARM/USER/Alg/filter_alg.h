#ifndef __FILTER_ALG_H
#define __FILTER_ALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"


//typedef struct 
//{
//	float lowpass_param;
//	float last_value;
//	float value;
//} WMA_filterdata;
	
typedef struct{
	
	float last_P;//上次估算协方差，初始化值为0.02
	float now_P;//档次估算协方差，初始化值为0

	float out;//滤波器输出，初始化值为0
	float Kg;//卡尔曼增益，初始化为0
	float Q;//过程噪声协方差，初始化值为0.001
	float R;//观测噪声协方差，初始化值为0.543
	float x_last;
	float x_now;
}kalman_filterdata;

typedef struct {
	float     group_data[8];
	int 			flag_data;
	float		  sum_group_data;
	float  		value;
}WMA_filterdata;

float WMA_filter(WMA_filterdata * pdata ,float pvalue);
float kalman_filter(kalman_filterdata* kfp,float input);
void WMA_filter_Init(WMA_filterdata* pdata);
void kalman_filter_Init(kalman_filterdata* pdata,float last_P,float now_P,float out,float Kg,float Q,float R,float x_last,float x_now);

#endif



