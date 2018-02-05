/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "ds18b20.h"


static __IO uint32_t TimingDelay;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIOB->BSRR = GPIO_Pin_12;
	/* Configure PB3 in output open drain mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	if (SysTick_Config(SystemCoreClock / 1000000))
	{
		/* Capture error */
		while (1);
	}

	OneWire_TypeDef OneWire = {
			.GPIOx = GPIOB,
			.GPIO_Pin = GPIO_Pin_12
	};
	OneWireStructInit(&OneWire);

	OneWire.Initialize(&OneWire);

	for(;;);
}


void OneWire_Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	while (TimingDelay > 0);
}

void SysTick_Handler (void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
