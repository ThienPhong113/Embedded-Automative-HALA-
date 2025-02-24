#include "main.h"

int main()
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	uint8_t dataTrans[4];
	while(1)
	{
		for(int i = 0; i < 4l; i++)
		{
			uint8_t temp = 0;
			while(temp == 0)
			{
				temp = SPI_Slave_Receive(0xFA);
				dataTrans[i] = temp;
			}
		}
	}
}

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 0xFFFF;
	timer.TIM_Prescaler = 36000 - 1;
	
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_Cmd(TIM2, ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}

uint8_t SPI_Slave_Receive(uint8_t byte_out)
{
	uint8_t byte_in = 0;
	//(1) Wait start condition
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	//(2) Wait data from Master
	for(int i = 0; i < 7; i++)
	{
		byte_in = (uint8_t)(byte_in << 1) | GPIO_ReadInputDataBit(SPI_GPIO ,SPI_MOSI_Pin);
    ((byte_out << i) & 0x80) ? GPIO_SetBits(SPI_GPIO, SPI_MISO_Pin) : GPIO_ResetBits(SPI_GPIO, SPI_MISO_Pin);
    while(GPIO_ReadInputDataBit(SPI_GPIO ,SPI_SCK_Pin));
    while(!GPIO_ReadInputDataBit(SPI_GPIO ,SPI_SCK_Pin));
	}
	//(3)Wait stop condition
  while(GPIO_ReadInputDataBit(SPI_GPIO ,SPI_SCK_Pin));
  while(!GPIO_ReadInputDataBit(SPI_GPIO ,SPI_CS_Pin));
	
	return byte_in;
}

void delay_ms(uint32_t ms)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < ms * 2);
}
