#include"pid_alg.h"


PID_PIDparam_Typedef PID_ChassisMotorpidparam;


void PID_InitParam(PID_PIDparam_Typedef* ppram, float kp, float ki, float kd, float max_pid, float max_sumi) {

    ppram->kp = kp;
    ppram->ki = ki;
    ppram->kd = kd;
    ppram->max_output = max_pid;
    ppram->max_sumi = max_sumi;
}

void PID_Init(PID_PIDTypeDef *ppid) {

    ppid->err[0]  = 0;
    ppid->err[1]  = 0;
    ppid->err[2]  = 0;
    ppid->err_lim = 0;
    ppid->sumi    = 0;
    ppid->output  = 0;
}

float PID_GetPIDOutput(PID_PIDTypeDef* ppid) {

    return ppid->output;
}

float PID_GetPIDRef(PID_PIDTypeDef* ppid) {
	
	return ppid->ref ;
}

void PID_SetPIDRef(PID_PIDTypeDef* ppid, float ref) {

    ppid->ref = ref;
}

void PID_SetPIDFdb(PID_PIDTypeDef* ppid, float fdb) {

    ppid->fdb = fdb;
}


void PID_ClearPID(PID_PIDTypeDef* ppid) {

//    ppid->ref = 0;
    ppid->fdb = 0;
    ppid->err[0] = 0;
    ppid->err[1] = 0;
    ppid->err[2] = 0;
    ppid->err_lim = 0;
    // ppid->err_fdf[0] = 0;
    // ppid->err_fdf[1] = 0;
    // ppid->err_fdf[2] = 0;
    // ppid->out_fdf    = 0;
    ppid->sumi        = 0;
    ppid->output     = 0;
 
}

void PID_SetPIDMax(PID_PIDparam_Typedef *pparam,float PID_Max) {
	
	pparam->max_output = PID_Max;
	
}
void PID_ClacPID(PID_PIDTypeDef* ppid, PID_PIDparam_Typedef* pparam) {

    float dError,Error;
    Error = ppid->ref - ppid->fdb;//计算当前误差
	
    ppid->err[2] = ppid->err[1];
    ppid->err[1] = ppid->err[0];
    ppid->err[0] = Error;

    dError = ppid->err[0] - ppid->err[1];
    ppid->sumi = Error +  ppid->err[1] + ppid->err[2];//位置式PID积分项累加
    if(ppid->sumi >= pparam->max_sumi)
        ppid->sumi = pparam->max_sumi;
		else if(ppid->sumi <= -pparam -> max_sumi)
				ppid->sumi = -pparam->max_sumi;
			
    float temp_output;
    temp_output = Error * pparam->kp + pparam->ki * ppid-> sumi + pparam->kd * dError;
    if(temp_output > pparam->max_output) {
        temp_output = pparam->max_output;
    }
    else if(temp_output<-pparam->max_output) {
        temp_output = -pparam->max_output;
    }
    
    ppid->output = temp_output;
}

