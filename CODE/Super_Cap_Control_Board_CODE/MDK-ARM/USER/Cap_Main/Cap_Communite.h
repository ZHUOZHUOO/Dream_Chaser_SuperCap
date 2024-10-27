#ifndef CAP_COMMUNITE_H
#define CAP_COMMUNITE_H

#ifdef __cplusplus
extern "C" {
#endif 
#include "main.h"
#include "configue.h"
#include "Cap_Main_Control.h"
	
 void Cap_Communicator_toBoss(uint8_t* rxdata);
 void Cap_SendMessage_Uart(void);
 void Cap_SendMessage_Can(void);



#endif
