#include"all_init.h"
#include"Cha_Chassis_Ctr.h"
#include"cmsis_os.h"
#include"motor_Periph.h"
#include"remote_Periph.h"


void Init_Task(void const * argument)
{
    Cha_ChassisInit();
    Power_Init();


    for(;;)
    {
        osDelay(1);
    }
}

void Power_Ctr_Switch(uint16_t POWER_PIN ,POWER_STATE powerstate) 
{
    HAL_GPIO_WritePin(GPIO_POWER_CTR_PORT,POWER_PIN,(GPIO_PinState)powerstate);
}

void Power_Init(void) {
        
    Power_Ctr_Switch(GPIO_POWER_CTR_PIN_1, POWER_ON);
    Power_Ctr_Switch(GPIO_POWER_CTR_PIN_2, POWER_ON);
    Power_Ctr_Switch(GPIO_POWER_CTR_PIN_3, POWER_ON);
    Power_Ctr_Switch(GPIO_POWER_CTR_PIN_4, POWER_ON);
}



