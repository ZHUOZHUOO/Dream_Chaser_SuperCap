#ifndef __ALL_INIT_H
#define __ALL_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include"main.h"

void Init_Task(void const * argument);

#define GPIO_POWER_CTR_PORT GPIOH
#define GPIO_POWER_CTR_PIN_1 GPIO_PIN_2
#define GPIO_POWER_CTR_PIN_2 GPIO_PIN_3
#define GPIO_POWER_CTR_PIN_3 GPIO_PIN_4
#define GPIO_POWER_CTR_PIN_4 GPIO_PIN_5

typedef enum {
    POWER_ON  = GPIO_PIN_SET,
    POWER_OFF = GPIO_PIN_RESET
}POWER_STATE;

#include"all_init.h"
#include"Cha_Chassis_Ctr.h"
#include"cmsis_os.h"
#include"motor_Periph.h"
#include"remote_Periph.h"

void Init_Task(void const * argument);
void Power_Ctr_Switch(uint16_t POWER_PIN ,POWER_STATE powerstate);
void Power_Init(void);






#endif

