/*
	�ö�ʱ����Դ����TIMER_NUM����ÿ������1ms������ʱ���ж�ʱ�䣩Ϊ��С��λ��

	��ʱ����ID��0~TIMER_NUM-1��ע��ID��Ҫ���������Χ��

	ʹ�ö�ʱ��ʱ����Ҫ������ʱ��������Timer_Create(uint8_t ID, uint16_t Time, uint8_t Mode)��Modeģʽ����δʵ�֡�
	�����ɹ��򷵻�0������ֵ��ʾ���ɹ��������궨ʱ��Ҫ����Timer_Start(uint8_t ID)������ʱ����	
	
	ע������Timer_Delete(uint8_t ID)����Ϊɾ����ʱ����ʱ������ζ�ʱ���ж�Ƭ��ʱ�䣬���ܻᵼ����ʧһ���жϡ�
	ɾ����ʱ���Ĺ�����Ҫ�ǴӶ�ʱ���������Ƴ����������Լ��ٶ�ʱ�������ɨ��ʱ�䡣
	����ö�ʱ�����������ã���ò�Ҫʹ��ɾ���������������Timer_ResetTime(uint8_t ID, uint16_t Time)�޸Ķ�ʱʱ�������á�

	ʹ��Timer_IsTimeOut(uint8_t ID)���Լ�鶨ʱʱ���Ƿ񵽴�����һ���ԵĶ�ʱ����Ҫ�ֶ����ʱ�������־λ��
	������Timer_ResetTimeOutFlag(uint8_t ID)����Ȼʹ��Timer_Restart(uint8_t ID)Ҳ���ԣ������������������������ʱ����
	��������ʺ������Զ�ʱ�������

*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "common.h"

#define TIMER_NUM 15	//��ʱ��������

#define T1MS (65536-FOSC/12/1000)   //1ms timer calculation method in 12T mode
#define T5MS (65536-5*FOSC/12/1000) //5ms

#define TIMEOUT T1MS  //��ʱ����װ��ֵ

//��ʱ������ṹ��
typedef struct _Timer_TypeDef
{
	uint8_t Flag;//��־λ
	/*
	bit7	��ʱ��ʱ�䵽	
	bit6	��ʱ������/ֹͣ

	bit1	��ʱ����ʹ�ã�1����ʹ��	0��δʹ��
	bit0	��ʱ������ģʽ��1���Զ���װ��	0���������У�δʵ�֣�
	*/
	uint16_t Count;//��ʱ��������
	uint16_t Time;//��ʱʱ�䣨1ms��λ��

	struct _Timer_TypeDef *Next;//ָ����һ����ʱ��
}Timer_TypeDef;

void Timer0_Init(void);
uint8_t Timer_Create(uint8_t ID, uint16_t Time, uint8_t Mode);
uint8_t Timer_Delete(uint8_t ID);
void Timer_Start(uint8_t ID);
void Timer_Restart(uint8_t ID);
void Timer_Stop(uint8_t ID);
void Timer_Reset(uint8_t ID);
void Timer_ResetTime(uint8_t ID, uint16_t Time);
uint16_t Timer_GetTime(uint8_t ID);
uint16_t Timer_GetCount(uint8_t ID);
uint8_t Timer_IsTimeOut(uint8_t ID);
void Timer_ResetTimeOutFlag(uint8_t ID);

#endif