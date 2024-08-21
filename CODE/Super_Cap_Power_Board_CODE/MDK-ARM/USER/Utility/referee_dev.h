/**
 *         clown File
 * 
 * File:        referee.h
 * Brief:       This document contains the data receiving and sending of the referee system
 * Author:      ???????
 * Modified:    2021/3/8      16?15?
 *
 */
#ifndef REFEREE_PERIPH_H
#define REFEREE_PERIPH_H
#ifdef __cplusplus
extern "C" {
#endif

#include "uart_util.h"
#include "crc_lib.h"
#include "string.h"

/********** START OF REFEREE CMD STRUCT DEFINITION **********/

typedef __packed struct {
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
} ext_game_status_t;

typedef __packed struct {
    uint8_t winner;
} ext_game_result_t;

typedef __packed struct {
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    uint16_t red_outpost_HP;
    uint16_t red_base_HP;
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    uint16_t blue_outpost_HP;
    uint16_t blue_base_HP;
} ext_game_robot_HP_t;

typedef __packed struct { 
    uint8_t dart_belong; 
    uint16_t stage_remaining_time; 
} ext_dart_status_t;

typedef __packed struct { 
    uint8_t F1_zone_status:1;
    uint8_t F1_zone_buff_debuff_status:3;
    uint8_t F2_zone_status:1;
    uint8_t F2_zone_buff_debuff_status:3;
    uint8_t F3_zone_status:1;
    uint8_t F3_zone_buff_debuff_status:3;
    uint8_t F4_zone_status:1;
    uint8_t F4_zone_buff_debuff_status:3;
    uint8_t F5_zone_status:1;
    uint8_t F5_zone_buff_debuff_status:3;
    uint8_t F6_zone_status:1;
    uint8_t F6_zone_buff_debuff_status:3;
} ext_ICRA_buff_debuff_zone_status_t;

typedef __packed struct {
    uint32_t event_type;
} ext_event_data_t;

typedef __packed struct {
    uint8_t supply_projectile_id; 
    uint8_t supply_robot_id; 
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

typedef __packed struct {
    uint8_t level;
    uint8_t foul_robot_id;
} ext_referee_warning_t;

typedef __packed struct {
    uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

/* (obsolete)
typedef __packed struct {
    uint8_t supply_projectile_id; 
    uint8_t supply_robot_id;
    uint8_t supply_num;
} ext_supply_projectile_booking_t;
*/

typedef __packed struct { 
    uint8_t robot_id;
    uint8_t robot_level;
    uint16_t remain_HP;
    uint16_t max_HP;
    uint16_t shooter_id1_17mm_cooling_rate;
    uint16_t shooter_id1_17mm_cooling_limit;
    uint16_t shooter_id1_17mm_speed_limit;
    uint16_t shooter_id2_17mm_cooling_rate;
    uint16_t shooter_id2_17mm_cooling_limit;
    uint16_t shooter_id2_17mm_speed_limit;
    uint16_t shooter_id1_42mm_cooling_rate;
    uint16_t shooter_id1_42mm_cooling_limit;
    uint16_t shooter_id1_42mm_speed_limit;
    uint16_t chassis_power_limit;
    uint8_t mains_power_gimbal_output : 1;
    uint8_t mains_power_chassis_output : 1;
    uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;

typedef __packed struct {
    uint16_t chassis_volt; 
    uint16_t chassis_current; 
    float chassis_power;
    uint16_t chassis_power_buffer; 
    uint16_t shooter_heat0; 
    uint16_t shooter_heat1;
    uint16_t shooter_id1_42mm_cooling_heat;
} ext_power_heat_data_t;

typedef __packed struct {
    float x; 
    float y; 
    float z; 
    float yaw;
} ext_game_robot_pos_t;

typedef __packed struct {
    uint8_t power_rune_buff;
} ext_buff_t;

typedef __packed struct {
    uint8_t energy_point; 
    uint8_t attack_time;
} aerial_robot_energy_t;

typedef __packed struct {
    uint8_t armor_id : 4; 
    uint8_t hurt_type : 4;
} ext_robot_hurt_t;

typedef __packed struct {
    uint8_t bullet_type; 
    uint8_t bullet_freq; 
	uint8_t shooter_id;
    float bullet_speed;
} ext_shoot_data_t;

typedef __packed struct {
    uint16_t bullet_remaining_num;
} ext_bullet_remaining_t;

typedef __packed struct { 
    uint32_t rfid_status;
} ext_rfid_status_t;

// ------------------------------

typedef __packed struct {
    uint16_t data_cmd_id; 
    uint16_t sender_ID; 
    uint16_t receiver_ID;
} ext_student_interactive_header_data_t;

/* (obsolete)
typedef __packed struct {
    float data1; 
    float data2; 
    float data3;
    uint8_t masks;
} client_custom_data_t;
*/

//typedef __packed struct {
//    uint8_t data[];
//} robot_interactive_data_t;

typedef __packed struct { 
    uint8_t operate_type; 
    uint8_t layer; 
} ext_client_custom_graphic_delete_t;

/* (obsolete)
typedef __packed struct {
    uint8_t operate_type; 
    uint8_t graphic_type; 
    uint8_t graphic_name[5]; 
    uint8_t layer;
    uint8_t color; 
    uint8_t width; 
    uint16_t start_x; 
    uint16_t start_y; 
    uint16_t radius; 
    uint16_t end_x; 
    uint16_t end_y; 
    int16_t start_angle; 
    int16_t end_angle; 
    uint8_t text_lenght; 
    uint8_t text[30];
} ext_client_graphic_draw_t;
*/

typedef __packed struct {
    uint8_t graphic_name[3]; 
    uint32_t operate_type:3; 
    uint32_t graphic_type:3; 
    uint32_t layer:4; 
    uint32_t color:4; 
    uint32_t start_angle:9; 
    uint32_t end_angle:9; 
    uint32_t width:10; 
    uint32_t start_x:11; 
    uint32_t start_y:11; 
    uint32_t radius:10; 
    uint32_t end_x:11; 
    uint32_t end_y:11;
} graphic_data_struct_t;

typedef __packed struct { 
    graphic_data_struct_t grapic_data_struct; 
} ext_client_custom_graphic_single_t;

typedef __packed struct { 
    graphic_data_struct_t grapic_data_struct[2]; 
} ext_client_custom_graphic_double_t;

typedef __packed struct { 
    graphic_data_struct_t grapic_data_struct[5]; 
} ext_client_custom_graphic_five_t;

typedef __packed struct { 
    graphic_data_struct_t grapic_data_struct[7]; 
} ext_client_custom_graphic_seven_t;

typedef __packed struct { 
    graphic_data_struct_t grapic_data_struct; 
    uint8_t data[30]; 
} ext_client_custom_character_t;

typedef __packed struct { 
		uint8_t dart_launch_opening_status;
		uint8_t dart_attack_target;
		uint16_t target_change_time;
		uint8_t first_dart_speed;
		uint8_t second_dart_speed;
		uint8_t third_dart_speed;
		uint8_t fourth_dart_speed;
		uint16_t last_dart_launch_time;
		uint16_t operate_launch_cmd_time; 
} ext_dart_client_cmd_t;
/********** END OF REFEREE CMD STRUCT DEFINITION **********/


typedef enum {
    Referee_STATE_NULL      = 0,
    Referee_STATE_CONNECTED = 1,
    Referee_STATE_LOST      = 2,
    Referee_STATE_ERROR     = 3,
    Referee_STATE_PENDING   = 4
} Referee_RefereeStateEnum;

typedef struct {
    Referee_RefereeStateEnum state;             // ????????
    uint32_t last_update_time;                  // ??????????
    
    uint16_t client_id;                         // ???ID
//  client_custom_data_t custom_data;           // (???)????????
//  ext_client_graphic_draw_t graphic_draw;     // (???)????????
    
    graphic_data_struct_t graphic_buf[30];      // ???????????
    uint8_t graphic_buf_len;                    // ????????????????
//  uint32_t graphic_current_id;                // ????????????
    
	uint8_t game_type;                        //  ????,    1:RoboMaster ?????;
	                                            //              2:RoboMaster ???????;
                                                //      	    3:ICRA RoboMaster ???????
	                                            //              4:RoboMaster ???3V3
	                                            //              5:RoboMaster ???1V1
    uint8_t game_progress;                      //  ??????,0:?????;
                                                //              1:????;
                                                //              2:????;
                                                //              3:5s???;
                                                //              4:???;
                                                //              5:?????
    uint16_t stage_remain_time;                 //  ????????,??s
    
	ext_game_robot_HP_t robot_HP;
	
    uint32_t event_type;                        
    
    uint8_t robot_id;
    uint8_t robot_level; 
    uint16_t remain_HP; 
    uint8_t max_chassis_power;
    uint8_t mains_power_gimbal_output; 
    uint8_t mains_power_chassis_output; 
    uint8_t mains_power_shooter_output;
    
    uint16_t chassis_volt; 
    uint16_t chassis_current; 
    float chassis_power;
    uint16_t chassis_power_buffer; 
    uint16_t shooter_heat0; 
    uint16_t shooter_heat1;
	uint16_t shooter_id1_42mm_cooling_heat;
	
    uint16_t shooter_heat0_cooling_rate;
    uint16_t shooter_heat1_cooling_rate;
    uint16_t shooter_heat0_cooling_limit;
    uint16_t shooter_heat1_cooling_limit;
    uint16_t shooter_heat0_speed_limit;
    uint16_t shooter_heat1_speed_limit;    
    uint16_t mobile_shooter_heat2;
	
	uint16_t shooter_id1_42mm_cooling_rate;
    uint16_t shooter_id1_42mm_cooling_limit;
    uint16_t shooter_id1_42mm_speed_limit;
    
    float x; 
    float y; 
    float z; 
    float yaw;
	float bullet_speed;
    uint8_t bullet_flag;
	
    uint8_t power_rune_buff;
	
	uint8_t  dart_launch_opening_status;
	uint8_t  dart_attack_target;
	uint16_t target_change_time;
	uint8_t  first_dart_speed;
	uint8_t  second_dart_speed;
	uint8_t  third_dart_speed;
	uint8_t  fourth_dart_speed;
	uint16_t last_dart_launch_time;
	uint16_t operate_launch_cmd_time;
    
} Referee_RefereeDataTypeDef;

typedef uint8_t (*Referee_RefereeCmdParseFuncDef)(Referee_RefereeDataTypeDef* referee, void *data_ptr);

typedef struct {
    uint16_t cmd_id;                            // ??ID
    uint8_t data_length;                        // ?????
    Referee_RefereeCmdParseFuncDef parse_func;  // ??????
} Referee_RefereeCmdTypeDef;

typedef struct {
    uint16_t robot_id;
    uint16_t client_id;
} Referee_RobotAndClientIDTypeDef;


typedef union {
    struct {
        uint32_t radius:10; 
        uint32_t end_x:11; 
        uint32_t end_y:11;
    } graphic_data;
    uint32_t ui32_data;
    float float_data;
    int32_t int_data;
} Referee_GraphicDataConverterUnion;

typedef enum {
    Draw_OPERATE_NULL   = 0,    // ???
    Draw_OPERATE_ADD    = 1,    // ??
    Draw_OPERATE_MODIFY = 2,    // ??
    Draw_OPERATE_DELETE = 3     // ??
} Draw_OperateType;             // ????

typedef enum {
    Draw_TYPE_LINE      = 0,    // ??
    Draw_TYPE_RECTANGLE = 1,    // ??
    Draw_TYPE_CIRCLE    = 2,    // ??
    Draw_TYPE_ELLIPSE   = 3,    // ??
    Draw_TYPE_ARC       = 4,    // ??
    Draw_TYPE_FLOAT     = 6,    // ???
    Draw_TYPE_INT       = 5,    // ???
    Draw_TYPE_STRING    = 7     // ??
} Draw_GraphicType;             // ????

typedef enum {
    Draw_COLOR_RED_BLUE  = 0,   // ????
    Draw_COLOR_YELLOW    = 1,   // ??
    Draw_COLOR_GREEN     = 2,   // ??
    Draw_COLOR_ORANGE    = 3,   // ??
    Draw_COLOR_VIOLET    = 4,   // ???
    Draw_COLOR_PINK      = 5,   // ??
    Draw_COLOR_CYAN      = 6,   // ??
    Draw_COLOR_BLACK     = 7,   // ??
    Draw_COLOR_WHITE     = 8    // ??
} Draw_Color;                   // ??


extern const uint16_t Const_Referee_RX_BUFF_LEN;
extern const uint16_t Const_Referee_REMOTE_OFFLINE_TIME;


extern Referee_RefereeDataTypeDef Referee_RefereeData;
extern UART_HandleTypeDef* Const_Referee_UART_HANDLER;

Referee_RefereeDataTypeDef* Referee_GetRefereeDataPtr(void);
void Referee_ResetRefereeData(void);
void Referee_InitReferee(UART_HandleTypeDef* huart);

uint16_t Referee_GetClientIDByRobotID(uint8_t robot_id);
void Referee_SendInteractiveData(uint16_t data_cmd_id, uint16_t receiver_ID, uint8_t *interactive_data, uint16_t interactive_data_length);
//void Referee_SetClientCustomDataLED(uint8_t led_no, uint8_t led_state);
//void Referee_SendClientCustomData(void);
void Referee_SendRobotCustomData(uint16_t data_cmd_id, uint16_t receiver_ID, uint8_t *data, uint16_t data_length);
//void Referee_SendClientGraphicsDraw(void);
void Referee_SendDrawingCmd(graphic_data_struct_t graph[], uint8_t mode);
void Referee_SendDrawingStringCmd(graphic_data_struct_t *pgraph, uint8_t str[]);
uint8_t Referee_IsDrawingBufferEmpty(void);
void Referee_DrawingBufferFlush(void);
void Referee_DrawingBufferPush(graphic_data_struct_t *graph);
void Referee_DrawingTimeBaseCallback(void);
uint32_t Referee_PackGraphicData(graphic_data_struct_t *pgraph, uint32_t graph_id, 
                                 Draw_OperateType operate_type, Draw_GraphicType graphic_type, uint8_t layer,
                                 Draw_Color color, uint16_t start_angle, uint16_t end_angle, 
                                 uint8_t width, uint16_t start_x, uint16_t start_y,
                                 uint16_t radius, uint16_t end_x, uint16_t end_y);
uint32_t Referee_PackFloatGraphicData(graphic_data_struct_t *pgraph, uint32_t graph_id, 
                                      Draw_OperateType operate_type, uint8_t layer,
                                      Draw_Color color, uint16_t font_size, uint16_t decimal_digit, 
                                      uint8_t width, uint16_t start_x, uint16_t start_y, float value);
uint32_t Referee_PackIntGraphicData(graphic_data_struct_t *pgraph, uint32_t graph_id, 
                                    Draw_OperateType operate_type, uint8_t layer,
                                    Draw_Color color, uint16_t font_size,
                                    uint8_t width, uint16_t start_x, uint16_t start_y, int value);
uint32_t Referee_PackStringGraphicData(graphic_data_struct_t *pgraph, uint32_t graph_id, 
                                       Draw_OperateType operate_type, uint8_t layer,
                                       Draw_Color color, uint16_t font_size, uint8_t length,
                                       uint8_t width, uint16_t start_x, uint16_t start_y);

void Draw_ClearLayer(uint8_t layer);
void Draw_ClearAll(void);
void Draw_Delete(uint32_t graph_id);
void Draw_AddLine(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                  uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
void Draw_ModifyLine(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                     uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
void Draw_AddRectangle(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                       uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
void Draw_ModifyRectangle(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                          uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
void Draw_AddCircle(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                    uint16_t center_x, uint16_t center_y, uint16_t radius);
void Draw_ModifyCircle(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                       uint16_t center_x, uint16_t center_y, uint16_t radius);
void Draw_AddEllipse(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                     uint16_t center_x, uint16_t center_y, uint16_t radius_x, uint16_t radius_y);
void Draw_ModifyEllipse(uint32_t graph_id, uint8_t layer, Draw_Color color, uint8_t width, 
                        uint16_t center_x, uint16_t center_y, uint16_t radius_x, uint16_t radius_y);
void Draw_AddArc(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t start_angle, uint16_t end_angle, 
                 uint8_t width, uint16_t center_x, uint16_t center_y, uint16_t radius_x, uint16_t radius_y);
void Draw_ModifyArc(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t start_angle, uint16_t end_angle, 
                    uint8_t width, uint16_t center_x, uint16_t center_y, uint16_t radius_x, uint16_t radius_y);
void Draw_AddFloat(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t font_size, uint16_t decimal_digit, 
                   uint8_t width, uint16_t start_x, uint16_t start_y, float value);
void Draw_ModifyFloat(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t font_size, uint16_t decimal_digit, 
                      uint8_t width, uint16_t start_x, uint16_t start_y, float value);
void Draw_AddInt(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t font_size,
                 uint8_t width, uint16_t start_x, uint16_t start_y, int value);
void Draw_ModifyInt(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t font_size,
                    uint8_t width, uint16_t start_x, uint16_t start_y, int value);
void Draw_AddString(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t font_size, 
                    uint8_t width, uint16_t start_x, uint16_t start_y, const char str[]);
void Draw_ModifyString(uint32_t graph_id, uint8_t layer, Draw_Color color, uint16_t font_size,
                       uint8_t width, uint16_t start_x, uint16_t start_y, const char str[]);

uint8_t Referee_IsRefereeOffline(void);
uint8_t Referee_CheckDataLengthByCmdID(uint16_t cmd_id, uint16_t data_length);
uint8_t Referee_ParseRobotCustomData(uint8_t* data, uint16_t data_length);
uint8_t Referee_ParseRefereeCmd(uint16_t cmd_id, uint8_t* data, uint16_t data_length);
void Referee_DecodeRefereeData(uint8_t* buff, uint16_t rxdatalen);
void Referee_RXCallback(UART_HandleTypeDef* huart);
void Referee_InitReferee(UART_HandleTypeDef* huart);
#endif
#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
