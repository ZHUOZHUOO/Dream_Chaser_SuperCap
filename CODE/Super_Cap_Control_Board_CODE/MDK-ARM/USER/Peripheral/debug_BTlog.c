/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \BTlog\debug_BTlog.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-31 09:16:32
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-01-18 17:11:42
 */

#include "debug_BTlog.h"
#include "string.h"
#include "Cap_Main_Control.h"
#include "Cap_Buck_Control.h"

#define ADD_SEND_DATA(x, y, z) AddSendData(&x, sizeof(x), y, z)
#define ADD_RECV_DATA(x, y) AddRecvData(&x, sizeof(x), y)

/*
* TODO: 
*   change huart 
*/
UART_HandleTypeDef* Const_BTlog_UART_HANDLER = &huart2;

//Device ID for APP
//Do not modify without request
const uint8_t Const_BTlog_ID = 0x03;

/*              Debug BTlog constant            */
/*
* TODO: 
*   change send break for logging rate
*/
#define Const_BTlog_HEART_SENT_PERIOD   5 // (ms)
#define Const_BTlog_RX_BUFF_LEN_MAX   500
#define Const_BTlog_TX_BUFF_LEN_MAX   500
#define Const_BTlog_RX_DATA_LEN_MAX   50
#define Const_BTlog_TX_DATA_LEN_MAX   50

uint8_t BTlog_RxData[Const_BTlog_RX_BUFF_LEN_MAX];
uint8_t BTlog_TxData[Const_BTlog_TX_BUFF_LEN_MAX];
BTlog_TableEntry BTlog_Send_Data[Const_BTlog_TX_DATA_LEN_MAX];
BTlog_TableEntry BTlog_Recv_Data[Const_BTlog_RX_DATA_LEN_MAX];

uint8_t BTlog_state_pending = 0;
uint8_t BTlog_state_sending = 0;
BTlog_TableEntry BTlog_Send_Data[Const_BTlog_TX_DATA_LEN_MAX];

uint8_t BTlog_startFlag = 0xfa;
char BTlog_endFlag[] = "@\r\n";
uint8_t BTlog_Recv_endFlag = 0x5a;

//StartFlag, Head and EndFlag
uint16_t BTlog_TX_BUFF_LEN = 3 + 1 + 3;
uint16_t BTlog_TX_DATA_LEN = 0;
//Head Checksum and EndFlag
uint16_t BTlog_RX_BUFF_LEN = 1 + 1 + 1;
uint16_t BTlog_RX_DATA_LEN = 0;

uint32_t BTlog_time = 0;

/**
 * @name: anonymous
 * @msg: 
 * @param {void*} ptr
 * @param {uint8_t} size
 * @param {BTlog_TypeEnum} type
 * @param {char*} tag
 * @return {*}
 */
void AddSendData(void* ptr, uint8_t size, BTlog_TypeEnum type, char* tag) {
    BTlog_Send_Data[BTlog_TX_DATA_LEN].ptr = ptr;
    BTlog_Send_Data[BTlog_TX_DATA_LEN].size = size;
    BTlog_Send_Data[BTlog_TX_DATA_LEN].type = type;
    memcpy(BTlog_Send_Data[BTlog_TX_DATA_LEN].tag, tag, BTlog_tagSize);
    BTlog_TX_BUFF_LEN += size;
    BTlog_TX_DATA_LEN++;
}

/**
 * @name: anonymous
 * @msg: 
 * @param {void*} ptr
 * @param {uint8_t} size
 * @param {BTlog_TypeEnum} type
 * @param {char*} tag
 * @return {*}
 */
void AddRecvData(void* ptr, uint8_t size, BTlog_TypeEnum type) {
    BTlog_Recv_Data[BTlog_RX_DATA_LEN].ptr = ptr;
    BTlog_Recv_Data[BTlog_RX_DATA_LEN].size = size;
    BTlog_Recv_Data[BTlog_RX_DATA_LEN].type = type;
    BTlog_RX_BUFF_LEN += size;
    BTlog_RX_DATA_LEN++;
}

/**
 * @name: INIT
 * @test: TODO: Add to init
 * @msg: 
 * @param {*}
 * @return {*}
 */

void BTlog_Init() {
    /*
    * TODO: 
    *   get access ptr for the data
    * eg. INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();
    */

    //Log Data Send
    ADD_SEND_DATA(BTlog_time, uInt32, "current_time");
		ADD_SEND_DATA(Cap_Adc_Data.Adc_Cap, Float, "cap_voltage");
		ADD_SEND_DATA(Cap_Adc_Data.Adc_Motor,Float, "adc_motor");
		//ADD_SEND_DATA(Cap_Adc_Data.Cap_Charging_Power,Float, "charging_power");
	//	ADD_SEND_DATA(Cap_Control_State.System_sum_Power,Float,"battery_power");
		//ADD_SEND_DATA(Cap_Control_State.Communicate_State,uInt8, "offline_state");
		ADD_SEND_DATA(Cap_Adc_Data.Motor_Power,Float, "motor_power");
		ADD_SEND_DATA(Cap_Control_State.BackFlush_State,uInt8,"Backflush_State");
		//ADD_SEND_DATA(Cap_Control_State.buff_power,uInt8,"buff_power");
		

    /*/
    * TODO: 
    *   ADD_SEND_DATA(%the varible%, %type%, %show_name%)
    *       Types currently supported:
    *           BYTE uInt8 uInt16 uInt32 Float Char Int8 Int16 Int32
    *       #Note that show_name should be no longer than 19 letters#
    * 
    * eg.    ADD_SEND_DATA(imu->angle.pitch, Float, "imu->angle.pitch");
    * eg.    ADD_SEND_DATA(Motor_chassisMotor1.encoder.speed, Float, "Chassis_Motor1_spd");
    * 
    */

    //Customize Remote Control Receive

    /*
    * TODO: 
    *   ADD_RECV_DATA(%the varible%, %type%)
    *       Types currently supported:
    *           BYTE uInt8 uInt16 uInt32 Float Char Int8 Int16 Int32
    * 
    * eg.    ADD_RECV_DATA(Chassis_Gyro_compensate[0], Float);
    * eg.    ADD_RECV_DATA(minipc->pitch_offset, Float);
    * 
    */
    Uart_DMA_Init(Const_BTlog_UART_HANDLER);
    Uart_ReceiveDMA(Const_BTlog_UART_HANDLER, BTlog_RxData, Const_BTlog_RX_BUFF_LEN_MAX);
}

/**
 * @name: SEND
 * @msg: TODO:  call this when needed
 * @param {*}
 * @return {*}
 */
void BTlog_Send() {
    if (BTlog_state_pending || !BTlog_state_sending)
        return;

    static uint32_t heart_count = 0;
    if ((HAL_GetTick() - heart_count) <= Const_BTlog_HEART_SENT_PERIOD)
        return;

    BTlog_time = HAL_GetTick();
    uint8_t* buff = BTlog_TxData;

    buff[0] = BTlog_startFlag;
    buff[1] = BTlog_startFlag;
    buff[2] = BTlog_startFlag;
    buff[3] = Const_BTlog_ID;
    int cur_pos = 4;
    for (uint16_t i = 0; i < BTlog_TX_DATA_LEN; i++) {
        uint8_t size = BTlog_Send_Data[i].size;
        memcpy(buff + cur_pos, BTlog_Send_Data[i].ptr, size);
        cur_pos += size;
    }
    memcpy(buff + cur_pos, BTlog_endFlag, sizeof(BTlog_endFlag) - 1);

    heart_count = HAL_GetTick();

    if (HAL_UART_GetState(Const_BTlog_UART_HANDLER) & 0x01)
        return;  // tx busy
    HAL_UART_Transmit_IT(Const_BTlog_UART_HANDLER, buff, BTlog_TX_BUFF_LEN);
}

const uint8_t CMD_GET_STRUCT = 0xFF;
const uint8_t CMD_START_SENDING = 0xF1;
const uint8_t CMD_STOP_SENDING = 0xF2;
// const uint8_t CMD_SET_GYRO_COMPENSATE = 0xA0;
const uint8_t CMD_SET_CUSTOMIZE = 0xA5;

/**
 * @name: DECODE
 * @msg: 
 * @param {uint8_t*} BTlog_RxData
 * @param {uint16_t} rxdatalen
 * @return {*}
 */
void BTlog_DecodeData(uint8_t* BTlog_RxData, uint16_t rxdatalen) {
    // HAL_UART_Transmit_IT(Const_BTlog_UART_HANDLER, BTlog_RxData, rxdatalen);
    if (rxdatalen == 1) {
        BTlog_state_pending = 1;
        if (BTlog_RxData[0] == CMD_GET_STRUCT) {
            int size = (BTlog_tagSize + 1) * BTlog_TX_DATA_LEN + 3 + 1 + sizeof(BTlog_endFlag);

            uint8_t* buff = BTlog_TxData;
            buff[0] = BTlog_startFlag;
            buff[1] = BTlog_startFlag;
            buff[2] = BTlog_startFlag;
            buff[3] = CMD_GET_STRUCT;
            for (uint16_t i = 0; i < BTlog_TX_DATA_LEN; i++) {
                buff[(BTlog_tagSize + 1) * i + 4] = BTlog_Send_Data[i].type & 0xff;
                memcpy(buff + (BTlog_tagSize + 1) * i + 1 + 4, BTlog_Send_Data[i].tag, BTlog_tagSize);
            }

            memcpy(buff + size - sizeof(BTlog_endFlag), BTlog_endFlag, sizeof(BTlog_endFlag) - 1);
           HAL_UART_Transmit_IT(Const_BTlog_UART_HANDLER, buff, size);
        } else if (BTlog_RxData[0] == CMD_START_SENDING) {
            BTlog_state_sending = 1;
        } else if (BTlog_RxData[0] == CMD_STOP_SENDING) {
            BTlog_state_sending = 0;
        }
        /*else if (BTlog_RxData[0] == CMD_SET_GYRO_COMPENSATE) {
            //float * 4
            Chassis_Gyro_compensate[0] = buff2float(BTlog_RxData + 1);
            Chassis_Gyro_compensate[1] = buff2float(BTlog_RxData + 5);
            Chassis_Gyro_compensate[2] = buff2float(BTlog_RxData + 9);
            Chassis_Gyro_compensate[3] = buff2float(BTlog_RxData + 13);
        }*/
        BTlog_state_pending = 0;
    } else {
        if (BTlog_RxData[0] == CMD_SET_CUSTOMIZE) {
            //check
            if (BTLog_VerifyData(BTlog_RxData, rxdatalen)) {
                uint8_t* buff = BTlog_RxData;
                int cur_pos = 1;
                for (uint16_t i = 0; i < BTlog_RX_DATA_LEN; i++) {
                    uint8_t size = BTlog_Recv_Data[i].size;
                    memcpy(BTlog_Recv_Data[i].ptr, buff + cur_pos, size);
                    cur_pos += size;
                }
            }
        }
    }
}
/**
  * @brief      Data Checksum Verify
  * @param      buff: Data buffer
  * @param      rxdatalen: recevie data length
  * @retval     Match is 1  not match is 0
  */

uint8_t BTLog_VerifyData(uint8_t* buff, uint16_t rxdatalen) {
    if (rxdatalen != BTlog_RX_BUFF_LEN)
        return 0;
    if (buff[0] != CMD_SET_CUSTOMIZE || buff[rxdatalen - 1] != BTlog_Recv_endFlag)
        return 0;

    uint8_t sum = buff[rxdatalen - 2];
    uint32_t checksum = 0;
    for (int i = 1; i < rxdatalen - 2; i++)
        checksum += buff[i];
    checksum = checksum & 0xff;
    return checksum == sum;
}

/**
 * @name: RX
 * @msg: TODO: add this to uart cbk
 * @param {UART_HandleTypeDef*} huart
 * @return {*}
 */
void BTlog_RXCallback(UART_HandleTypeDef* huart) {
   // __HAL_DMA_DISABLE(huart->hdmarx);
    uint16_t rxdatalen = Const_BTlog_RX_BUFF_LEN_MAX - Uart_DMACurrentDataCounter(huart->hdmarx->Instance);

    BTlog_DecodeData(BTlog_RxData, rxdatalen);
		huart->hdmarx->Instance->CNDTR = Const_BTlog_RX_BUFF_LEN_MAX;
    //__HAL_DMA_SET_COUNTER(huart->hdmarx, Const_BTlog_RX_BUFF_LEN_MAX);
   // __HAL_DMA_ENABLE(huart->hdmarx);
}
