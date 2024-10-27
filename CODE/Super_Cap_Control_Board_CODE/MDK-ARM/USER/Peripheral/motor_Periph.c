#include"motor_Periph.h"
#include"pid_alg.h"
#include"cmsis_os.h"
#include"can_util.h"

Motor_MotorGroupTypedef Motor_chassisMotors;
Motor_MotorTypedef Motor_chassisLeft1;
Motor_MotorTypedef Motor_chassisLeft2;
Motor_MotorTypedef Motor_chassisRight1;
Motor_MotorTypedef Motor_chassisRight2;
PID_PIDparam_Typedef Motor_chassismotorpidparam = {20,0.1,0.01,30000,200000};
uint8_t txdata_test[8] = {0};
int16_t value;
int16_t counter_value = 0;
float power = 0;
uint8_t flag=0;




void Motor_SendMessage(Motor_MotorGroupTypedef* pmotor) {

  if (pmotor->motor_type == Motor_CANMOTOR) {
    CAN_TxHeaderTypeDef pheader;
    Can_InitTxHander(&pheader,CAN_ID_STD , 0x200, 8, 0);
    pheader.TransmitGlobalTime = DISABLE;
    uint8_t txdata[8];
    value = (int16_t)(pmotor->motor_motorhandler[0].motor_pidhander.output);
    txdata[0] = (uint8_t)(((int16_t)(pmotor->motor_motorhandler[0].motor_pidhander.output)) >> 8);
    txdata[1] = (uint8_t)(pmotor->motor_motorhandler[0].motor_pidhander.output);
    txdata[2] = (uint8_t)(((int16_t)(pmotor->motor_motorhandler[1].motor_pidhander.output)) >> 8);
    txdata[3] = (uint8_t)(pmotor->motor_motorhandler[1].motor_pidhander.output);
    txdata[4] = (uint8_t)(((int16_t)(pmotor->motor_motorhandler[2].motor_pidhander.output)) >> 8);
    txdata[5] = (uint8_t)(pmotor->motor_motorhandler[2].motor_pidhander.output);
    txdata[6] = (uint8_t)(((int16_t)(pmotor->motor_motorhandler[3].motor_pidhander.output)) >> 8);
    txdata[7] = (uint8_t)(pmotor->motor_motorhandler[3].motor_pidhander.output);

    for(int num = 0;num<=7;num++) {
      txdata_test[num] = txdata[num];
    }
    
    
    HAL_CAN_AddTxMessage(pmotor->phcan, &pheader, txdata, (uint32_t *)CAN_TX_MAILBOX0);
  }
  else if (pmotor->motor_type == Motor_PWMMOTOR) {

  }
  else {

  }
}

void Motor_MotorEncoderDecode(uint8_t rxdata[], Motor_MotorTypedef *pmotor) {

  pmotor->angle = rxdata[0] << 8 | rxdata[1];
  pmotor->speed = rxdata[2] << 8 | rxdata[3];
  pmotor->current = rxdata[4] << 8 | rxdata[5];
  pmotor->temp = rxdata[6];
  pmotor->motor_pidhander.fdb = pmotor->speed;
  if(flag == 1) 
  pmotor->motor_pidhander.ref = 0;

  if(flag == 1&&pmotor->speed != 0) {
      power+=-pmotor->current*24.0*20*0.001/16384;
      if(pmotor->speed>=100)
      counter_value++;
  }

}

void Motor_MotorGroupsPID_Clac(Motor_MotorGroupTypedef *pmotor) {

  uint8_t motor_num =  pmotor->motor_number ;
  for(;motor_num>0;motor_num--) {
    Motor_MotorPID_Clac(& pmotor->motor_motorhandler[motor_num-1]);
  }
}

void Motor_MotorPID_Clac(Motor_MotorTypedef *pmotor) {
  
  PID_ClacPID(& pmotor->motor_pidhander,& pmotor->motor_pidparamhander);
}

void Motor_MotorGroupInit(Motor_MotorGroupTypedef* pmotor, uint8_t motor_num, Motor_MotorTypeEnum motors_type, CAN_HandleTypeDef *hcan, TIM_HandleTypeDef *htim) {

  pmotor->motor_number = motor_num;
  pmotor->motor_type = motors_type;
  pmotor->phcan = hcan;
  pmotor->ptim = htim;

  pmotor->motor_motorhandler[0] = Motor_chassisLeft1;
  pmotor->motor_motorhandler[1] = Motor_chassisLeft2;
  pmotor->motor_motorhandler[2] = Motor_chassisRight1;
  pmotor->motor_motorhandler[3] = Motor_chassisRight2;
  
  Motor_motorInit(& pmotor->motor_motorhandler[0], motors_type, Motor_chassismotorpidparam);
  Motor_motorInit(& pmotor->motor_motorhandler[1], motors_type, Motor_chassismotorpidparam);
  Motor_motorInit(& pmotor->motor_motorhandler[2], motors_type, Motor_chassismotorpidparam);
  Motor_motorInit(& pmotor->motor_motorhandler[3], motors_type, Motor_chassismotorpidparam);
}

void Motor_motorInit(Motor_MotorTypedef * pmotor, Motor_MotorTypeEnum motortype, PID_PIDparam_Typedef motorpidparam) {
  
  pmotor->angle = 0;
  pmotor->current = 0;
  pmotor->temp = 0;
  pmotor->speed = 0;

  pmotor->last_angle = 0;
  pmotor->limited_angle = 0;
  pmotor->motor_pidparamhander = motorpidparam; 
  pmotor->has_init = Motor_HAS_INIT;
  pmotor->consequent_angle = 0;
  pmotor->motor_type = motortype;
  pmotor->init_offset = 0;
  pmotor->motor_pidparamhander = motorpidparam;
  PID_InitPID(& pmotor->motor_pidhander);
}

void Motor_Chassis_Decoder(Motor_MotorGroupTypedef* pmotor, CAN_HandleTypeDef *phcan )
{
  CAN_RxHeaderTypeDef rxhander;
  uint8_t Can_RxData[8];
  uint32_t ret = HAL_CAN_GetRxMessage(phcan, CAN_RX_FIFO0, &rxhander, Can_RxData);
  switch(rxhander.StdId) {
    case 0x201:
    Motor_MotorEncoderDecode(Can_RxData, &pmotor->motor_motorhandler[0]);
    break;
    case 0x202:
    Motor_MotorEncoderDecode(Can_RxData, &pmotor->motor_motorhandler[1]);
    break;
    case 0x203:
    Motor_MotorEncoderDecode(Can_RxData, &pmotor->motor_motorhandler[2]);
    break;
    case 0x204:
    Motor_MotorEncoderDecode(Can_RxData, &pmotor->motor_motorhandler[3]);
    break;
  }

}

void Task_MotorControl(void const * argument)
{
  /* USER CODE BEGIN Task_MotorControl */
	Can_IntFilterAndStart(&hcan1);
  Motor_MotorGroupInit(&Motor_chassisMotors,4,Motor_CANMOTOR, &hcan1,(TIM_HandleTypeDef *)NULL);
  /* Infinite loop */
  for(;;)
  {
    Motor_MotorGroupsPID_Clac( &Motor_chassisMotors);
    Motor_SendMessage(&Motor_chassisMotors);
    osDelay(1);
  }
  /* USER CODE END Task_MotorControl */
}









