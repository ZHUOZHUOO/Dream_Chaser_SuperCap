#ifndef __REMOTE_PERIPH_H
#define __REMOTE_PERIPH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include"usart.h"
 #include"uart_util.h"
#define const_remote_uarthander huart1
extern uint8_t Remote_rxdata[18];

typedef enum{
    Remote_STATE_NULL          = 0,
    Remote_STATE_UNCONNECTED   = 1,
    Remote_STATE_CONNECTED     = 2          
} Remote_RemoteStateEnum;

typedef enum{
    Remote_SW_NULL = 0,
    Remote_SW_UP = 1,
    Remote_SW_MIDDLE = 3,
    Remote_SW_DOWN = 2
} Remote_SW_STATE;

typedef struct{
    struct {
        int16_t ch0;
        int16_t ch1;
        int16_t ch2;
        int16_t ch3;
        Remote_SW_STATE sw_L;
        Remote_SW_STATE sw_R;
    } Remote_remote;

    struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } Remote_mouse;
    struct {
        uint8_t press_bottom;
    } Remote_key;
  } Remote_RemoteCtr_Data;

	
extern UART_HandleTypeDef* Const_Remote_UART_HANDLER;
extern uint8_t remote_rxdata[18];
extern Remote_RemoteCtr_Data Remote_remotectr_data;

void Remote_DecodeRemoteData(uint8_t rx[]);
void Remote_RxCallback(uint8_t* Remote_rxdata);



#endif
