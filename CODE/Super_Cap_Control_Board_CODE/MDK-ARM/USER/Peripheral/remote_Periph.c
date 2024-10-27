#include"remote_Periph.h"
#include "cmsis_os.h"

UART_HandleTypeDef* Const_Remote_UART_HANDLER = &huart3;
uint8_t Remote_rxdata[18];
Remote_RemoteCtr_Data Remote_remotectr_data;


void Remote_DecodeRemoteData(uint8_t rx[]) {

    if (rx == NULL)
    {
        return;
    }
    Remote_remotectr_data.Remote_remote.ch0 =(((uint16_t)rx[0] | (uint16_t)rx[1] << 8) & 0x7FF)-1024;
    Remote_remotectr_data.Remote_remote.ch1 = (((uint16_t)rx[1] >> 3 | (uint16_t)rx[2] << 5) & 0x7FF)-1024 ;
    Remote_remotectr_data.Remote_remote.ch2=(int16_t)(((uint16_t)rx[2] >> 6 | (uint16_t)rx[3] << 2 | (uint16_t)rx[4] << 10) & 0x07FF) - 1024;
    Remote_remotectr_data.Remote_remote.ch3 = (int16_t)(((uint16_t)rx[4] >> 1 | (uint16_t)rx[5] << 7) & 0x07FF) - 1024;
    Remote_remotectr_data.Remote_remote.sw_L = (Remote_SW_STATE)((rx[5] >> 4) & 0x03);
    Remote_remotectr_data.Remote_remote.sw_R = (Remote_SW_STATE)((rx[5] >> 6) & 0x03);
    Remote_remotectr_data.Remote_mouse.x = ((uint16_t)rx[6]) | (uint16_t)rx[7] << 8;
    Remote_remotectr_data.Remote_mouse.y = ((uint16_t)rx[8]) | (uint16_t)rx[9] << 8;
    Remote_remotectr_data.Remote_mouse.z = ((uint16_t)rx[10]) | (uint16_t)rx[11] << 8;
    Remote_remotectr_data.Remote_mouse.press_l = (uint8_t)rx[12];
    Remote_remotectr_data.Remote_mouse.press_r = (uint8_t)rx[13];
    Remote_remotectr_data.Remote_key.press_bottom = (uint16_t)rx[14]|(uint16_t)rx[15]<<8;
}

void Remote_RxCallback(uint8_t* Remote_rxdata) {

    Remote_DecodeRemoteData(Remote_rxdata);
}

void task_Remote(void const * argument) {
	
	for(;;)
  {
		Remote_DecodeRemoteData(Remote_rxdata);
    osDelay(7);
  }
}





