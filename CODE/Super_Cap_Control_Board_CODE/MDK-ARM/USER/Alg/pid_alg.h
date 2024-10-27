#ifndef PID_ALG_H
#define PID_ALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"

typedef struct {
    float ref;
    float fdb;
    float err[3];
    float err_lim;          // Integral anti-windup 
    // float err_fdf[3];       // Feedforard
    // float out_fdf;          // Feedforard out put
    float sumi;
    float output;

    // Filter_LowPassTypeDef d_fil;
    // Filter_LowPassTypeDef delta_fil;

    // Filter_LowPassTypeDef kf1_fil;
    // Filter_LowPassTypeDef kf2_fil;
    float err_watch;
} PID_PIDTypeDef;

typedef struct {
    float kp;
    float ki;
    float kd;

    float max_output;
    float max_sumi;   
} PID_PIDparam_Typedef;

extern void PID_InitPIDParam(PID_PIDparam_Typedef* ppram, float kp, float ki, float kd, float max_pid, float max_sumi);
float PID_GetPIDOutput(PID_PIDTypeDef* ppid);
void PID_SetPIDRef(PID_PIDTypeDef* ppid, float ref);
void PID_ClearPID(PID_PIDTypeDef* ppid);
void PID_ClacPID(PID_PIDTypeDef* ppid, PID_PIDparam_Typedef* pparam);
void PID_Init(PID_PIDTypeDef *ppid);
void PID_SetPIDFdb(PID_PIDTypeDef* ppid, float fdb);
float PID_GetPIDRef(PID_PIDTypeDef* ppid) ;
void PID_SetPIDMax(PID_PIDparam_Typedef *pparam,float PID_Max);

#endif

