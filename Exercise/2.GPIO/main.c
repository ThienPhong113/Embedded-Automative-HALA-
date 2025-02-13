#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void delay(uint32_t time)
{
	uint32_t i = 0;
	for(; i < time; i++);
}

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Config()
{
	GPIO_InitTypeDef led;
	
	led.GPIO_Pin = GPIO_Pin_13;
	led.GPIO_Mode = GPIO_Mode_Out_PP;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &led);
}


int main()
{
	RCC_Config();
	GPIO_Config();
	
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(1000000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(1000000);
	}
}
