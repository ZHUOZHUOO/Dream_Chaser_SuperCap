#ifndef QUEUE_STR_H
#define QUEUE_STR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"

typedef struct {
		float float_array[100];
		float float_array_backup[100];
		uint32_t length;
		float avg;
		uint32_t tick;
}Queue_Str_Typedef;

float Queue_Getavg(Queue_Str_Typedef * pqueue);
void Queue_Init(Queue_Str_Typedef * pqueue,uint32_t length);
void Queue_Push(Queue_Str_Typedef * pqueue,float num);
#endif

