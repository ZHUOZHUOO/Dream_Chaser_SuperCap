/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : This document contains the OLED font library
 *  Description  : font_lib.h
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-07-09 03:37:58
 *  LastEditTime : 2021-07-16 10:36:48
 */

#ifndef FONT_LIB_H
#define FONT_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx.h"

extern const unsigned char asc2_1206[95][12];
extern const unsigned char OLED_LOGO_RM[128][8];
extern const unsigned char OLED_BEAR[1024];
extern const unsigned char OLED_SUR[1024];
extern const unsigned char OLED_SUB[1024];

#ifdef __cplusplus
}
#endif

#endif
