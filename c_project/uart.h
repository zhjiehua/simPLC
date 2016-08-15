/*
	串口1波特率最高4800
	该串口解析程序只能实现半双通信，而且接收到的数据格式必须是0x0D 0x0A结尾
	串口解析协议请看下面解析部分的说明
	串口缓冲区大小不能大于32
*/

#ifndef __UART_H__
#define __UART_H__

#include "common.h"

#define UART_BUFFER_SIZE  15//串口缓冲区大小，不能大于32
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
	uint8_t Flag;//串口标志位
	/*
		bit7	串口接收完成
		bit6	串口接收到0x0D
		bit5~0	串口接收有效字节数目
	*/
	uint8_t *pBuf;//缓冲区指针
}Uart_Man_TypeDef;

extern Uart_Man_TypeDef Uart_Man;

void Uart_Init(void);
void Uart_SendData(uint8_t dat);
void Uart_SendString(uint8_t *s);

/*--------------------------------数据解析部分-------------------------------------

	数据解析的协议如下
	BYTE0	动作类型	如GET、SET等
	BYTE1	动作对象	如X、Y、M、T等
	BYTE2	位置(低8)		
	BYTE3	位置(高8)	对象位置
	BYTE4	参数(低8)	
	BYTE5	参数(高8)	值
	BYTE6	校验和      BYTE6 = BYTE0 ^ BYTE1 ^ BYTE2 ^ BYTE3 ^ BYTE4 ^ BYTE5

					BYTE 0	 1	 2	 3	 4	 5	 6
	比如 SET Y1 = 1，	02	02	01	00	01	00	XX
		 SET T10 = 100  02	04	0A	00	64	00	XX	
*/

#define CHECK_SUM 0//是否使用校验和，调试时不建议用

#define GET	0x01
#define SET 0x02

#define X	0x00
#define Y	0x01
#define M	0x02
#define T	0x03

void Uart_DataParse(void);

#endif