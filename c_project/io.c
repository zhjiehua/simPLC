#include "io.h"

idata uint8_t IN_State[IN_NUM];//������˲���
/*
	bit8 ��¼�����ƽ
	bit7 ������ʱ�Ŀ���
	bit6 ��ʱʱ�䵽��־
	bit5~0 ��ʱ��ʱ������
*/
static idata uint8_t IN_Last[(IN_NUM+7)/8];//�������һ״̬
static idata uint8_t IN[(IN_NUM+7)/8];//�����
static idata uint8_t IN_Rising[(IN_NUM+7)/8];//�����������
static idata uint8_t IN_Falling[(IN_NUM+7)/8];//������½���

static idata uint8_t OUT_Last[(OUT_NUM+7)/8];//�������һ״̬
static idata uint8_t OUT[(OUT_NUM+7)/8];//�����
static idata uint8_t OUT_Rising[(OUT_NUM+7)/8];//�����������
static idata uint8_t OUT_Falling[(OUT_NUM+7)/8];//������½���

static idata uint8_t RELAY_Last[(RELAY_NUM+7)/8];//�м�̵�����һ״̬
static idata uint8_t RELAY[(RELAY_NUM+7)/8];//�м�̵���
static idata uint8_t RELAY_Rising[(RELAY_NUM+7)/8];//�м�̵���������
static idata uint8_t RELAY_Falling[(RELAY_NUM+7)/8];//�м�̵����½���

//IO����ṹ��
IO_Man_TypeDef IO_Man;

//IO��ʼ��
void IO_Init(void)
{
	IO_Man.X_Level = IN;
	IO_Man.Y_Level = OUT;
	IO_Man.M_Level = RELAY;
	
	IO_Man.X_FallingEdge = IN_Falling;
	IO_Man.Y_FallingEdge = OUT_Falling;
	IO_Man.M_FallingEdge = RELAY_Falling;
	
	IO_Man.X_RisingEdge = IN_Rising;
	IO_Man.Y_RisingEdge = OUT_Rising;
	IO_Man.M_RisingEdge = RELAY_Rising;	
}

//��ȡ����㣬����������
static void _GetX(uint8_t X, uint8_t Num)
{
	uint8_t cIndex = Num / 8;
	uint8_t rIndex = Num % 8;

	if(!X != (IN_State[Num]>>7) & 0x01)
	{
		//��¼�����ƽ��������ʱ
		if(!X)
			IN_State[Num] = 0x80|0x40;	
		else
			IN_State[Num] = 0x40;
	}
	if(IN_State[Num] & 0x20)//��ʱʱ�䵽
	{
		IN_State[Num] &= ~0x20;
		if(!X == (IN_State[Num]>>7) & 0x01)//�������֮ǰ�ĵ�ƽ����ʾ�������Ч
		{
			//IN_Last[cIndex] = IN[cIndex];//��¼��һ��״̬����Ҫ��ɨ��ǰ����
			if(!X)
				IN[cIndex] |= 0x01<<rIndex;	
			else
				IN[cIndex] &= ~(0x01<<rIndex);
		}
	}
}

//ˢ��IO��
void IO_Refresh(void)
{
	uint8_t i;	
/*----------------------------�����---------------------------------*/
	//��¼�������һ״̬
	for(i=0;i<(IN_NUM+7)/8;i++)
		IN_Last[i] = IN[i];

	//��ȡ�����
	_GetX(X0, 0);
	_GetX(X1, 1);
	_GetX(X2, 2);
	_GetX(X3, 3);
	_GetX(X4, 4);
	_GetX(X5, 5);
	_GetX(X6, 6);
	_GetX(X7, 7);
	_GetX(X10, 8);
	_GetX(X11, 9);
	_GetX(X12, 10);
	_GetX(X13, 11);
	
	for(i=0;i<(IN_NUM+7)/8;i++)
	{
		IN_Rising[i] = (IN_Last[i]^IN[i]) & IN[i];
		IN_Falling[i] = (IN_Last[i]^IN[i]) & IN_Last[i];
	}

/*----------------------------�����----------------------------------*/
	//�����ˢ��
	P1 = ~OUT[0];

	for(i=0;i<(OUT_NUM+7)/8;i++)
	{
		OUT_Rising[i] = (OUT_Last[i]^OUT[i]) & OUT[i];
		OUT_Falling[i] = (OUT_Last[i]^OUT[i]) & OUT_Last[i];
		OUT_Last[i] = OUT[i];
	}

/*----------------------------�м�̵���----------------------------------*/
	for(i=0;i<(RELAY_NUM+7)/8;i++)
	{
		RELAY_Rising[i] = (RELAY_Last[i]^RELAY[i]) & RELAY[i];
		RELAY_Falling[i] = (RELAY_Last[i]^RELAY[i]) & RELAY_Last[i];
		RELAY_Last[i] = RELAY[i];
	}
}

//----------------------------IO_API-------------------------------
//��IO
//Componet Ԫ�����ͣ���ȡX_COMPONET Y_COMPONET M_COMPONET
//Index Ԫ����� 
//Type ��ƽ���ͣ���ȡLEVEL FALLING_EDGE RISING_EDGE
uint8_t IO_Get(uint8_t Component, uint8_t Index, uint8_t Type)
{
	uint8_t **p = &IO_Man.X_Level;
	return (uint8_t)((*(p + Component + 3*Type))[Index/8]>>(Index%8) & 0x01);		
}

//дIO
void IO_Set(uint8_t Component, uint8_t Index, uint8_t Level)
{
	uint8_t **p = &IO_Man.X_Level;
	if(Level)
		(*(p + Component))[Index/8] |= (0x01<<(Index%8));
	else
		(*(p + Component))[Index/8] &= ~(0x01<<(Index%8));		
}