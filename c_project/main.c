#include "uart.h"
#include "timer.h"
#include "io.h"
#include "eeprom.h"

//-----------------------------------------------

/* main program */
void main()
{	
	uint16_t i;
//---------------初始化------------------------------------
	IO_Init();
	Uart_Init();//串口初始化
	Timer0_Init(); //定时器初始化

	WDT_CONTR = 0x3C;//开看门狗，溢出时间1.1377s

    EA = 1;//开启全局中断

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

		//喂狗
		WDT_CONTR = 0x3C;

		//刷新IO点
		IO_Refresh();

		//解析串口数据
		Uart_DataParse();

//	   	for(i=0;i<8;i++)
//			IO_Set(Y_COMPONET, i, IO_Get(X_COMPONET, i, LEVEL));

	}
#endif
}

