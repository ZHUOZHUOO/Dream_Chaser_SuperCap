#ifndef __MOTOR_PERIPH_H
#define __MOTOR_PERIPH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "uart_util.h"
#include "stdlib.h"
#include "pid_alg.h"
#include "fdcan_util.h"

#define const_chassismotor_canhander hcan1


typedef enum {
    Motor_UNCONNECTED = 0,
    Motor_CANMOTOR = 1,
    Motor_PWMMOTOR = 2
} Motor_MotorTypeEnum;

typedef enum {
    Motor_DIS_INIT = 0,
    Motor_HAS_INIT = 1
} Motor_MotorHasInit;

typedef struct {
    int16_t angle;
    int16_t speed;
    int16_t current;
    int16_t temp;

    int16_t last_angle;
    int16_t round_counter;
    Motor_MotorHasInit has_init;
    Motor_MotorTypeEnum motor_type;
    float init_offset;
    float limited_angle;
    float consequent_angle;

    PID_PIDparam_Typedef motor_pidparamhander;
    PID_PIDTypeDef motor_pidhander;

} Motor_MotorTypedef;

typedef struct {
    uint8_t motor_number;
    Motor_MotorTypeEnum motor_type;
    Motor_MotorTypedef motor_motorhandler[4];
    CAN_HandleTypeDef * phcan;
    TIM_HandleTypeDef * ptim;

} Motor_MotorGroupTypedef;

extern Motor_MotorGroupTypedef Motor_chassisMotors;


void Motor_SendMessage(Motor_MotorGroupTypedef* pmotor);
void Motor_MotorEncoderDecode(uint8_t rxdata[], Motor_MotorTypedef *pmotor);
void Motor_MotorGroupsPID_Clac(Motor_MotorGroupTypedef *pmotor);
void Motor_MotorPID_Clac(Motor_MotorTypedef *pmotor);
void Motor_MotorGroupInit(Motor_MotorGroupTypedef* pmotor, uint8_t motor_num, Motor_MotorTypeEnum motors_type, CAN_HandleTypeDef *hcan, TIM_HandleTypeDef *htim);
void Motor_motorInit(Motor_MotorTypedef * pmotor, Motor_MotorTypeEnum motortype, PID_PIDparam_Typedef motorpidparam);
void Motor_Chassis_Decoder(Motor_MotorGroupTypedef* pmotor, CAN_HandleTypeDef *phcan);






#endif

