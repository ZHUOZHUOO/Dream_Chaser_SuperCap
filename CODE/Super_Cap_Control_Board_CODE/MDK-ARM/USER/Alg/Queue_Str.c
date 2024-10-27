#include"Queue_Str.h"
#include"string.h"
float Queue_Getavg(Queue_Str_Typedef * pqueue) {

    return pqueue->avg;
}

void Queue_Init(Queue_Str_Typedef * pqueue,uint32_t length) {

    pqueue->length = length;
		pqueue->tick = 0;
    //pqueue->float_array = (float*)malloc(length * sizeof(float));
		//pqueue->float_array_backup = (float*)malloc(length * sizeof(float));
}

void Queue_Push(Queue_Str_Typedef * pqueue,float num) {
		
    pqueue->avg = (pqueue->avg * pqueue->length - pqueue->float_array[pqueue->tick] + num) / pqueue->length;		
    pqueue->float_array_backup[pqueue->tick] = num;
		pqueue->tick++;
		if(pqueue->tick>=100) {
			memcpy(pqueue->float_array,pqueue->float_array_backup,pqueue->length * sizeof(float));
			pqueue->tick = 0;
		}
	
}
