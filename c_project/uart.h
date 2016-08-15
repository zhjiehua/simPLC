/*
	����1���������4800
	�ô��ڽ�������ֻ��ʵ�ְ�˫ͨ�ţ����ҽ��յ������ݸ�ʽ������0x0D 0x0A��β
	���ڽ���Э���뿴����������ֵ�˵��
	���ڻ�������С���ܴ���32
*/

#ifndef __UART_H__
#define __UART_H__

#include "common.h"

#define UART_BUFFER_SIZE  15//���ڻ�������С�����ܴ���32
#define BAUD 4800           //UART baudrate

/*Define UART parity mode*/
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity

#define PARITYBIT NONE_PARITY   //Testing even parity

typedef struct
{
	uint8_t Flag;//���ڱ�־λ
	/*
		bit7	���ڽ������
		bit6	���ڽ��յ�0x0D
		bit5~0	���ڽ�����Ч�ֽ���Ŀ
	*/
	uint8_t *pBuf;//������ָ��
}Uart_Man_TypeDef;

extern Uart_Man_TypeDef Uart_Man;

void Uart_Init(void);
void Uart_SendData(uint8_t dat);
void Uart_SendString(uint8_t *s);

/*--------------------------------���ݽ�������-------------------------------------

	���ݽ�����Э������
	BYTE0	��������	��GET��SET��
	BYTE1	��������	��X��Y��M��T��
	BYTE2	λ��(��8)		
	BYTE3	λ��(��8)	����λ��
	BYTE4	����(��8)	
	BYTE5	����(��8)	ֵ
	BYTE6	У���      BYTE6 = BYTE0 ^ BYTE1 ^ BYTE2 ^ BYTE3 ^ BYTE4 ^ BYTE5

					BYTE 0	 1	 2	 3	 4	 5	 6
	���� SET Y1 = 1��	02	02	01	00	01	00	XX
		 SET T10 = 100  02	04	0A	00	64	00	XX	
*/

#define CHECK_SUM 0//�Ƿ�ʹ��У��ͣ�����ʱ��������

#define GET	0x01
#define SET 0x02

#define X	0x00
#define Y	0x01
#define M	0x02
#define T	0x03

void Uart_DataParse(void);

#endif