#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}


void GPIO_Config()
{
	GPIO_InitTypeDef led;
	
	led.GPIO_Pin = GPIO_Pin_13;
	led.GPIO_Mode = GPIO_Mode_Out_PP;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &led);
}

void TIM_Config()
{
	TIM_TimeBaseInitTypeDef timer2;
	
	timer2.TIM_Prescaler = 7200 - 1;
	timer2.TIM_Period = 0xFFFF;
	timer2.TIM_CounterMode = TIM_CounterMode_Up;
	timer2.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2, &timer2);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint32_t s)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < s);
}
int main()
 {
	 RCC_Config();
	GPIO_Config();
	TIM_Config();
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay_ms(5000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay_ms(5000);
	}
}
