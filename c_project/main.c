#include "uart.h"
#include "timer.h"
#include "io.h"
#include "eeprom.h"

//-----------------------------------------------

/* main program */
void main()
{	
	uint16_t i;
//---------------��ʼ��------------------------------------
	IO_Init();
	Uart_Init();//���ڳ�ʼ��
	Timer0_Init(); //��ʱ����ʼ��

	WDT_CONTR = 0x3C;//�����Ź������ʱ��1.1377s

    EA = 1;//����ȫ���ж�

   	if(IapReadByte(IAP_ADDRESS) != 0xA5)
	{
		IapEraseSector(IAP_ADDRESS);    //Erase current sector
		IapProgramByte(IAP_ADDRESS, 0xA5);
		Uart_SendString("eeprom erase!\r\n");
	}

#if 1
    while (1)                       //loop
	{
		uint8_t i;

		//ι��
		WDT_CONTR = 0x3C;

		//ˢ��IO��
		IO_Refresh();

		//������������
		Uart_DataParse();

//	   	for(i=0;i<8;i++)
//			IO_Set(Y_COMPONET, i, IO_Get(X_COMPONET, i, LEVEL));

	}
#endif
}

