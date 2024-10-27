#ifndef __ERROR_HANDLE_H
#define __ERROR_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif 
#include"main.h"

void Communicate_Offline_Error_Handle(void);
void Short_Circuit_Error_Handle(void);
void RxError_Handler(void);
void Motor_Error_Handle(void);
#endif
