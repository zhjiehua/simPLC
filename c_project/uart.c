#include "uart.h"
#include "io.h"
#include "timer.h"

//���ڹ���ṹ��
Uart_Man_TypeDef Uart_Man;

//���ڻ�����
static idata uint8_t Uart_Buf[UART_BUFFER_SIZE];

//��־�������ڷ�������
static bit Uart_Busy;

//���ڳ�ʼ�������ڲ����ʷ������õ��Ƕ�ʱ��1
void Uart_Init(void)
{
	//PCON |= 0x80;	//�����ʼӱ�

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;            //8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;            //9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;            //9-bit variable UART, parity bit initial to 0
#endif
	TMOD |= 0x20;            //Set Timer1 as 8-bit auto reload mode
	TH1 = TL1 = 256 - (FOSC/12/32/BAUD); //Set auto-reload vaule ��9600�������ò���
	TR1 = 1;                //Timer1 start run
	ES = 1;                 //Enable UART interrupt

	Uart_Man.Flag = 0;
	Uart_Man.pBuf = Uart_Buf;
}

/*----------------------------
UART interrupt service routine
----------------------------*/
void Uart_Isr() interrupt 4 using 1
{
	uint8_t Buf = SBUF;

    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag

		//---------------------------------------------------------
		if((Uart_Man.Flag & 0x80) == 0x00)//����δ���
		{
			if(Uart_Man.Flag & 0x40)//���յ���0x0D
			{
				if(Buf != 0x0A)
					Uart_Man.Flag = 0;//���մ���
				else
					Uart_Man.Flag |= 0x80;//�������	
			}
			else //��û�յ�0X0D
			{
				if(Buf == 0x0D)
				{
					Uart_Man.Flag |= 0x40;
				}
				else
				{
					Uart_Man.pBuf[Uart_Man.Flag & 0x1F] =  Buf;
					Uart_Man.Flag++;

					if(Uart_Man.Flag > (UART_BUFFER_SIZE - 1))
						Uart_Man.Flag = 0;//�������ݴ���,���¿�ʼ����	  
				}
			}
		}
		//---------------------------------------------------------
    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        Uart_Busy = 0;           //Clear transmit busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: Data (data to be sent)
Output:None
----------------------------*/
void Uart_SendData(uint8_t Data)
{
    while (Uart_Busy);           //Wait for the completion of the previous data is sent
    ACC = Data;              //Calculate the even parity bit P (PSW.0)
    if (P)                  //Set the parity bit according to P
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;            //Set parity bit to 0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;            //Set parity bit to 1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;            //Set parity bit to 1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;            //Set parity bit to 0
#endif
    }
    Uart_Busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void Uart_SendString(uint8_t *s)
{
    while (*s)              //Check the end of the string
    {
        Uart_SendData(*s++);     //Send current char and increment string ptr
    }
}

//�������ݽ���
void Uart_DataParse(void)
{
	uint16_t temp;

#if CHECK_SUM
	uint8_t sum_t;
	uint8_t i;
	uint8_t num = Uart_Man.Flag & 0x1F;
	for(i=0;i<(num-2);i++)
	  sum_t ^= Uart_Man.pBuf[i];
	
	if(sum_t != Uart_Man.pBuf[num-1])
	{
		Uart_Man.Flag = 0;//���½���
		return;
	}	
#endif

	if(Uart_Man.Flag & 0x80)//������ڽ����������
	{
		//Uart_SendString("Uart parse data\r\n");
		switch(Uart_Man.pBuf[0])
		{
			case GET://GET	
				switch(Uart_Man.pBuf[1])
				{
//					case X:
//						Uart_SendData(IO_GetX(Uart_Man.pBuf[2]));
//						break;
//					case Y:
//						Uart_SendData(IO_GetY(Uart_Man.pBuf[2]));
//						break;
//					case M:
//						break;
					case X:
					case Y:
					case M:
						Uart_SendData(IO_Get(Uart_Man.pBuf[1], Uart_Man.pBuf[2], LEVEL));
						break;
					case T:
						temp = Timer_GetTime(Uart_Man.pBuf[2]);
						Uart_SendData((uint8_t)temp);
						Uart_SendData((uint8_t)(temp>>8));
						break;
					default:
						break;
				}	
				break;//GET
			case SET://SET
				switch(Uart_Man.pBuf[1])
				{
//					case X:
//						Uart_Man.pBuf[4] ? IO_SetX(Uart_Man.pBuf[2]) : IO_ClearX(Uart_Man.pBuf[2]);
//						Uart_SendData(0xAA);
//						break;
//					case Y:
//						Uart_Man.pBuf[4] ? IO_SetY(Uart_Man.pBuf[2]) : IO_ClearY(Uart_Man.pBuf[2]);
//						Uart_SendData(0xAA);
//						break;
//					case M:
//						break;
					case X:
					case Y:
					case M:
						IO_Set(Uart_Man.pBuf[1], Uart_Man.pBuf[2], Uart_Man.pBuf[4]);
						Uart_SendData(0xAA);
						break;
					case T:
						Timer_ResetTime(Uart_Man.pBuf[2], ((uint16_t)Uart_Man.pBuf[5])<<8 | (uint16_t)Uart_Man.pBuf[4]);
						Uart_SendData(0xAA);
						break;
					default:
						break;
				}
				break;//SET
		}//switch(Uart_Man.pBuf[0])
		Uart_Man.Flag = 0;//���½���			
	}//������ڽ����������
}