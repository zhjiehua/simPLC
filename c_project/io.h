/*
	IN_NUM�������������ĸ�����������˳��Ϊ0~IN_NUM��10���ƣ�.
	����PLC�е�X10 X11����ʵ����X8 X9

	OUT_NUM�������������ĸ�����������˳��Ϊ0~OUT_NUM��10���ƣ�.

	IO_Refresh()��������ˢ������IO��״̬��������������������������������Ҫ��while(1)�е���

	���ĺ���_GetX(uint8_t X, uint8_t Num)���ڶ�ȡ����㣬�����˲����ܡ�

	�˲�ʱ����SHAKE_TIME*IO_FRE_DOUBLE����ʱ���ڣ�����SHAKE_TIME��ΧΪ0~31.
	���ڵ��˲�ʱ����SHAKE_TIME*IO_FRE_DOUBLE��1ms
*/

#ifndef __IO_H__
#define __IO_H__

#include "common.h"

#define SHAKE_TIME 32//32����ʱ����
#define IO_FRE_DOUBLE 1//�����õĶ�ʱ��Ƶ

#define IN_NUM 12 //�������
#define OUT_NUM 8  //�������
#define RELAY_NUM 50 //�м�̵�������

//Ԫ������
#define X_COMPONET	0
#define Y_COMPONET	1
#define M_COMPONET	2

//��ƽ����
#define LEVEL			0
#define FALLING_EDGE	1
#define RISING_EDGE		2

//IO�㶨�����
typedef struct
{
	//��ƽ
	uint8_t *X_Level;
	uint8_t *Y_Level;
	uint8_t *M_Level;

	//�½���
	uint8_t *X_FallingEdge;
	uint8_t *Y_FallingEdge;
	uint8_t *M_FallingEdge;

	//������
	uint8_t *X_RisingEdge;
	uint8_t *Y_RisingEdge;
	uint8_t *M_RisingEdge;
}IO_Man_TypeDef;

//���������
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

//���������
sbit Y0 = P1^0;
sbit Y1 = P1^1;
sbit Y2 = P1^2;
sbit Y3 = P1^3;
sbit Y4 = P1^4;
sbit Y5 = P1^5;
sbit Y6 = P1^6;
sbit Y7 = P1^7;

extern idata uint8_t IN_State[IN_NUM];//������˲���
/*
	bit8 ��¼�����ƽ
	bit7 ������ʱ�Ŀ���
	bit6 ��ʱʱ�䵽��־
	bit5~0 ��ʱ��ʱ������
*/

void IO_Init(void);
void IO_Refresh(void);

uint8_t IO_Get(uint8_t Component, uint8_t Index, uint8_t Type);
void IO_Set(uint8_t Component, uint8_t Index, uint8_t Level);

#endif