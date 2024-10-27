#include "oled_periph.h"


#include "i2c_util.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "cmsis_os.h"
#include "stdio.h"

I2C_HandleTypeDef* Const_OLED_I2C_HANDLER = &hi2c1;

 uint8_t OLED_GRAM[128][8];
char Adc_Judge_To_Char[15];
char Adc_Motor_To_Char[15];
char JudgePower_To_Char[15];
char Motor_Current_To_Char[15];
uint8_t has_start = 0;

//		W J D delete OLED faction 2023/3/11


//void Task_Oled_Show(void const * argument)
//{
//	
//  for(;;)
//  {	
//	if(HAL_GetTick() >= 100 && has_start == 0) {
//		OLED_init(); 
//		OLED_DisplayLogo();
//		has_start = 1;
//	}
//	
//	if(HAL_GetTick() >= 3000) {
//		if(HAL_GetTick() <= 3020) {
//				OLED_Clear();
//		}

//		
////		sprintf(Adc_Judge_To_Char, "%9.6f",Adc_Vcc );
////		Adc_Judge_To_Char[10] = 'V';
////		sprintf(Adc_Motor_To_Char, "%9.6f",Adc_Motor );
////		Adc_Motor_To_Char[10] = 'V';
////		sprintf(JudgePower_To_Char, "%9.5f",Cap_Control_State.ref_power );
////		JudgePower_To_Char[10] = 'W';
////		sprintf(Motor_Current_To_Char, "%9.5f",Adc_Current_Motor );
////		Motor_Current_To_Char[10] = 'A';
////		if(Cap_Control_State.Communicate_State == Communicate_Online ) {
////				OLED_ShowString(1,1,"bantong online");
////		}else {
////			OLED_ShowString(1,1,"bantong offline");
////		}
////		OLED_ShowString(2,1,"judge_vo ");
////		OLED_ShowString(2,10,Adc_Judge_To_Char);
////		OLED_ShowString(3,1,"motor_vo ");
////		OLED_ShowString(3,10,Adc_Motor_To_Char);
////		OLED_ShowString(4,1,"judgepower ");
////		OLED_ShowString(4,10,JudgePower_To_Char);
////		OLED_ShowString(4,1,"motorcur ");
////		OLED_ShowString(4,10,Motor_Current_To_Char);
////		OLED_RefreshGram();
//		}
//		
//    osDelay(5);
//  }
//  /* USER CODE END Task_Oled_Show */
//}

/**
  * @brief          Write data/command to OLED, if you use spi, please rewrite the function
  * @param          date: the data ready to write
  * @param          cmd: OLED_CMD means command; OLED_DATA means data
  * @retval         NULL
  */
uint8_t cmd_data[2];
void Oled_Writebyte(uint8_t date, uint8_t cmd) {
    
    if (cmd == OLED_CMD) {
        cmd_data[0] = 0x00;
    } else {
        cmd_data[0] = 0x40;
    }
    cmd_data[1] = date;
    I2c_MasterSendMessage(Const_OLED_I2C_HANDLER, OLED_I2C_ADDRESS, cmd_data, 2);
}

/**
  * @brief          Initialize the oled device
  * @param          NULL
  * @retval         NULL
  */
void OLED_init() {
    Oled_Writebyte(0xAE, OLED_CMD);  // display off
    Oled_Writebyte(0x20, OLED_CMD);  // Set Memory Addressing Mode
    Oled_Writebyte(0x10, OLED_CMD);  // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    Oled_Writebyte(0xb0, OLED_CMD);  // Set Page Start Address for Page Addressing Mode,0-7
    Oled_Writebyte(0xc8, OLED_CMD);  // Set COM Output Scan Direction
    Oled_Writebyte(0x00, OLED_CMD);  // ---set low column address
    Oled_Writebyte(0x10, OLED_CMD);  // ---set high column address
    Oled_Writebyte(0x40, OLED_CMD);  // --set start line address
    Oled_Writebyte(0x81, OLED_CMD);  // --set contrast control register
    Oled_Writebyte(0xff, OLED_CMD);  // brightness 0x00~0xff
    Oled_Writebyte(0xa1, OLED_CMD);  // --set segment re-map 0 to 127
    Oled_Writebyte(0xa6, OLED_CMD);  // --set normal display
    Oled_Writebyte(0xa8, OLED_CMD);  // --set multiplex ratio(1 to 64)
    Oled_Writebyte(0x3F, OLED_CMD);  //
    Oled_Writebyte(0xa4, OLED_CMD);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    Oled_Writebyte(0xd3, OLED_CMD);  // -set display offset
    Oled_Writebyte(0x00, OLED_CMD);  // -not offset
    Oled_Writebyte(0xd5, OLED_CMD);  // --set display clock divide ratio/oscillator frequency
    Oled_Writebyte(0xf0, OLED_CMD);  // --set divide ratio
    Oled_Writebyte(0xd9, OLED_CMD);  // --set pre-charge period
    Oled_Writebyte(0x22, OLED_CMD);  //
    Oled_Writebyte(0xda, OLED_CMD);  // --set com pins hardware configuration
    Oled_Writebyte(0x12, OLED_CMD);
    Oled_Writebyte(0xdb, OLED_CMD);  // --set vcomh
    Oled_Writebyte(0x20, OLED_CMD);  // 0x20,0.77xVcc
    Oled_Writebyte(0x8d, OLED_CMD);  // --set DC-DC enable
    Oled_Writebyte(0x14, OLED_CMD);  //
    Oled_Writebyte(0xaf, OLED_CMD);  // --turn on oled panel
}

/**
  * @brief          Turn on OLED display
  * @param          NULL
  * @retval         NULL
  */
void OLED_DisplayOn() {
    Oled_Writebyte(0x8d, OLED_CMD);
    Oled_Writebyte(0x14, OLED_CMD);
    Oled_Writebyte(0xaf, OLED_CMD);
}

/**
  * @brief          Turn off OLED display
  * @param          NULL
  * @retval         NULL
  */
void OLED_DisplayOff() {
    Oled_Writebyte(0x8d, OLED_CMD);
    Oled_Writebyte(0x10, OLED_CMD);
    Oled_Writebyte(0xae, OLED_CMD);
}

/**
  * @brief          Operate the graphic ram(size: 128*8 char)
  * @param          pen: the type of operate.
  * @param          PEN_CLEAR: set ram to 0x00
  * @param          PEN_WRITE: set ram to 0xff
  * @param          PEN_INVERSION: bit inversion 
  * @retval         NULL
  */
void OLED_OperateGram(Oled_PenEnum pen) 
{
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        for (n = 0; n < 128; n++) {
            if (pen == PEN_WRITE) {
                OLED_GRAM[n][i] = 0xff;
            } else if (pen == PEN_CLEAR) {
                OLED_GRAM[n][i] = 0x00;
            } else {
                OLED_GRAM[n][i] = 0xff - OLED_GRAM[n][i];
            }
        }
    }
}

/**
  * @brief          Cursor set to (x,y) point
  * @param          x:X-axis, from 0 to 127
  * @param          y:Y-axis, from 0 to 7
  * @retval         NULL
  */
void OLED_SetPos(uint8_t x, uint8_t y) {
    Oled_Writebyte((0xb0 + y), OLED_CMD);                // set page address y
    Oled_Writebyte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);  // set column high address
    Oled_Writebyte((x & 0x0f), OLED_CMD);                // set column low address
}

/**
  * @brief          Draw one bit of graphic raw, operate one point of screan(128*64)
  * @param          x: x-axis, [0, X_WIDTH-1]
  * @param          y: y-axis, [0, Y_WIDTH-1]
  * @param          pen: type of operation,
  * @param          PEN_CLEAR: set (x,y) to 0
  * @param          PEN_WRITE: set (x,y) to 1
  * @param          PEN_INVERSION: (x,y) value inversion 
  * @retval         NULL
  */
void OLED_DrawPoint(int8_t x, int8_t y, Oled_PenEnum pen) {
    uint8_t page = 0, row = 0;

    /* check the corrdinate */
    if ((x < 0) || (x > (X_WIDTH - 1)) || (y < 0) || (y > (Y_WIDTH - 1))) {
        return;
    }
    page = y / 8;
    row = y % 8;

    if (pen == PEN_WRITE) {
        OLED_GRAM[x][page] |= 1 << row;
    } else if (pen == PEN_INVERSION) {
        OLED_GRAM[x][page] ^= 1 << row;
    } else {
        OLED_GRAM[x][page] &= ~(1 << row);
    }
}

/**
  * @brief          Draw a line from (x1, y1) to (x2, y2)
  * @param          x1: the start point of line
  * @param          y1: the start point of line
  * @param          x2: the end point of line
  * @param          y2: the end point of line
  * @param          pen: type of operation,PEN_CLEAR,PEN_WRITE,PEN_INVERSION.
  * @retval         NULL
  */
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Oled_PenEnum pen)
{
    uint8_t col = 0, row = 0;
    uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
    float k = 0.0f, b = 0.0f;

    if (y1 == y2) {
        (x1 <= x2) ? (x_st = x1) : (x_st = x2);
        (x1 <= x2) ? (x_ed = x2) : (x_ed = x1);

        for (col = x_st; col <= x_ed; col++) {
            OLED_DrawPoint(col, y1, pen);
        }
    } else if (x1 == x2) {
        (y1 <= y2) ? (y_st = y1) : (y_st = y2);
        (y1 <= y2) ? (y_ed = y2) : (y_ed = y1);

        for (row = y_st; row <= y_ed; row++) {
            OLED_DrawPoint(x1, row, pen);
        }
    } else {
        k = ((float)(y2 - y1)) / (x2 - x1);
        b = (float)y1 - k * x1;

        (x1 <= x2) ? (x_st = x1) : (x_st = x2);
        (x1 <= x2) ? (x_ed = x2) : (x_ed = x2);

        for (col = x_st; col <= x_ed; col++) {
            OLED_DrawPoint(col, (uint8_t)(col * k + b), pen);
        }
    }
}

/**
  * @brief          Show a character
  * @param          row: start row of character
  * @param          col: start column of character
  * @param          chr: the character ready to show
  * @retval         NULL
  */
void OLED_ShowChar(uint8_t row, uint8_t col, char chr) {
    uint8_t x = col * 6;
    uint8_t y = row * 12;
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr = chr - ' ';

    for (t = 0; t < 12; t++) {
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; t1++) {
            if (temp & 0x80)
                OLED_DrawPoint(x, y, PEN_WRITE);
            else
                OLED_DrawPoint(x, y, PEN_CLEAR);

            temp <<= 1;
            y++;
            if ((y - y0) == 12) {
                y = y0;
                x++;
                break;
            }
        }
    }
}

/**
  * @brief          Show a character string
  * @param          row: row of character string begin
  * @param          col: column of character string begin
  * @param          chr: the pointer to character string
  * @retval         NULL
  */
void OLED_ShowString(uint8_t row, uint8_t col, char* chr) {
    uint8_t n = 0;

    while (chr[n] != '\0') {
        OLED_ShowChar(row, col, chr[n]);
        col++;

        if (col > 20) {
            col = 0;
            row += 1;
        }
        n++;
    }
}

/**
  * @brief          Formatted output in oled 128*64
  * @param          row: row of character string begin, 0 <= row <= 4;
  * @param          col: column of character string begin, 0 <= col <= 20;
  * @param          *fmt: the pointer to format character string
  * @note           if the character length is more than one row at a time, the extra characters will be truncated
  * @retval         NULL
  */
void OLED_Printf(uint8_t row, uint8_t col, const char* fmt, ...) {
    static char LCD_BUF[128] = {0};
    static va_list ap;
    uint8_t remain_size = 0;

    if ((row > 4) || (col > 20)) {
        return;
    }
    va_start(ap, fmt);

    vsprintf((char*)LCD_BUF, fmt, ap);

    va_end(ap);

    remain_size = 21 - col;

    LCD_BUF[remain_size] = '\0';

    OLED_ShowString(row, col, LCD_BUF);
}

/**
  * @brief          Send the data of gram to oled sreen
  * @param          NULL
  * @retval         NULL
  */
void OLED_RefreshGram() {
    uint8_t i, n;

    for (i = 0; i < 8; i++) {
        OLED_SetPos(0, i);
        for (n = 0; n < 128; n++) {
            Oled_Writebyte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

/**
  * @brief          Show the logo of RoboMaster
  * @param          NULL
  * @retval         NULL
  */
void OLED_DisplayLogo() {
    uint8_t temp_char = 0;
    uint8_t x = 0, y = 0;
    uint8_t i = 0;
    OLED_OperateGram(PEN_CLEAR);

    for (; y < 64; y += 8) {
        for (x = 0; x < 128; x++) {
            temp_char = OLED_LOGO_RM[x][y / 8];
            for (i = 0; i < 8; i++) {
                if (temp_char & 0x80) {
                    OLED_DrawPoint(x, y + i, PEN_WRITE);
                } else {
                    OLED_DrawPoint(x, y + i, PEN_CLEAR);
                }
                temp_char <<= 1;
            }
        }
    }
    OLED_RefreshGram();
}

/**
  * @brief          Show the BMG
  * @param          NULL
  * @retval         NULL
  */
void OLED_DisplayBMG(const unsigned char* buf) {
    uint8_t j, i;
    uint16_t n = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 128; j++) {
            OLED_GRAM[j][i] = buf[n++];
        }
    }
}

/**
  * @brief          Show the GIF
  * @param          NULL
  * @retval         NULL
  */
void OLED_DisplayGIF(const unsigned char gif[][1024]) {
    static uint32_t frames_period = 0;
    static uint8_t cur_frame = 0;
    uint32_t frame = 80;

    if (cur_frame >= 15) {
        cur_frame = 15;
        frames_period = HAL_GetTick();
        OLED_DisplayBMG(gif[cur_frame]);
        frames_period++;
        cur_frame = 0;
    } else if ((HAL_GetTick() - frames_period) >= frame) {
        frames_period = HAL_GetTick();
        OLED_DisplayBMG(gif[cur_frame]);
        cur_frame++;
    }
}

void OLED_Clear(void) {
 
  memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
  OLED_RefreshGram() ;//¸üÐÂÏÔÊ¾
}
