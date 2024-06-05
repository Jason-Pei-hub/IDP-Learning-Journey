#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h" 
#include "sys.h" 
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);//显示一个字符
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);//显示字符串
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);//显示十进制数字
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);//显示有符号十进制数字
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);//显示十六进制数字
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);//显示二进制数字

#endif
