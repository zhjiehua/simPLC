/*
	IN_NUM用于配置输入点的个数，输入点的顺序为0~IN_NUM（10进制）.
	对于PLC中的X10 X11等其实就是X8 X9

	OUT_NUM用于配置输出点的个数，输出点的顺序为0~OUT_NUM（10进制）.

	IO_Refresh()函数用于刷新所有IO的状态，包括所有输入输出。所以这个函数需要在while(1)中调用

	核心函数_GetX(uint8_t X, uint8_t Num)用于读取输入点，包括滤波功能。

	滤波时间是SHAKE_TIME*IO_FRE_DOUBLE个定时周期，其中SHAKE_TIME范围为0~31.
	现在的滤波时间是SHAKE_TIME*IO_FRE_DOUBLE个1ms
*/

#ifndef __IO_H__
#define __IO_H__

#include "common.h"

#define SHAKE_TIME 32//32个定时周期
#define IO_FRE_DOUBLE 1//消抖用的定时倍频

#define IN_NUM 12 //输入点数
#define OUT_NUM 8  //输出点数
#define RELAY_NUM 50 //中间继电器个数

//元件类型
#define X_COMPONET	0
#define Y_COMPONET	1
#define M_COMPONET	2

//电平类型
#define LEVEL			0
#define FALLING_EDGE	1
#define RISING_EDGE		2

//IO点定义管理
typedef struct
{
	//电平
	uint8_t *X_Level;
	uint8_t *Y_Level;
	uint8_t *M_Level;

	//下降沿
	uint8_t *X_FallingEdge;
	uint8_t *Y_FallingEdge;
	uint8_t *M_FallingEdge;

	//上升沿
	uint8_t *X_RisingEdge;
	uint8_t *Y_RisingEdge;
	uint8_t *M_RisingEdge;
}IO_Man_TypeDef;

//定义输入点
sbit X0 = P0^0;
sbit X1 = P0^1;
sbit X2 = P0^2;
sbit X3 = P0^3;
sbit X4 = P0^4;
sbit X5 = P0^5;
sbit X6 = P0^6;
sbit X7 = P0^7;
sbit X10 = P2^7;
sbit X11 = P2^6;
sbit X12 = P2^5;
sbit X13 = P2^4;

//定义输出点
sbit Y0 = P1^0;
sbit Y1 = P1^1;
sbit Y2 = P1^2;
sbit Y3 = P1^3;
sbit Y4 = P1^4;
sbit Y5 = P1^5;
sbit Y6 = P1^6;
sbit Y7 = P1^7;

extern idata uint8_t IN_State[IN_NUM];//输入点滤波用
/*
	bit8 记录输入电平
	bit7 开启延时的开关
	bit6 延时时间到标志
	bit5~0 延时定时周期数
*/

void IO_Init(void);
void IO_Refresh(void);

uint8_t IO_Get(uint8_t Component, uint8_t Index, uint8_t Type);
void IO_Set(uint8_t Component, uint8_t Index, uint8_t Level);

#endif