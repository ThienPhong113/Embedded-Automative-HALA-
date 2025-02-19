#include "main.h"

int main()
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	
	I2C_Init();
	while(1)
	{
		
	}
}


void I2C_Init()
{
	SDA_1;
	SCL_1;
}
void I2C_Start()
{
	SDA_0;
	delay_us(I2C_Cycle);
	SCL_0;
	delay_us(I2C_Cycle);
}
void I2C_setAddress(uint8_t addr)
{
	for(int i = 0; i < 7; i++)
	{
		((addr << i) & 0x80) ? SDA_1 : SDA_0;
		SCL_1;
		delay_us(0.5 * I2C_Cycle);
		SCL_0;
		delay_us(0.5 * I2C_Cycle);		
	}
}
void I2C_sendByte(uint8_t byte)
{
	for(int i = 0; i < 8; i++)
	{
		
	}
}
void I2C_Stop()
{
	
}

void RCC_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_ClockDivision = TIM_CKD_DIV1; 
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 36000 - 1;
	timer.TIM_Prescaler = 2 - 1;
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_Cmd(TIM2, ENABLE);
	
	timer.TIM_ClockDivision = TIM_CKD_DIV1; 
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 72 - 1;
	timer.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInit(TIM3, &timer);
	TIM_Cmd(TIM3, ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO;
	GPIO.GPIO_Mode 	=	GPIO_Mode_Out_OD;
	GPIO.GPIO_Pin  	=	I2C_SCL | I2C_SDA;	
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO);
}

void delay_ms(uint32_t ms)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < ms * 36);
}
void delay_us(uint32_t us)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < us * 72);
}
