#include "main.h"

int main()
{
	
}


void RCC_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef timer;
	
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 999;
	timer.TIM_Prescaler = 7;
	
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_Cmd(TIM2, ENABLE);	
}
void GPIO_Config()
{	
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	GPIO_InitStructure.GPIO_Pin = SPI1_CS| SPI1_SCK| SPI1_MISO| SPI1_MOSI; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}
void SPI1_Config()
{
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	
	SPI_Init(SPI1, &SPI_InitStructure); 
	SPI_Cmd(SPI1, ENABLE); 
}
void SPI_Send1Byte(uint8_t data)
{ 
	GPIO_WriteBit(SPI1_GPIO, SPI1_CS, Bit_RESET); 
	SPI_I2S_SendData(SPI1, data); 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==0); 
	GPIO_WriteBit(SPI1_GPIO, SPI1_CS, Bit_SET); 
} 
uint8_t SPI_Receive1Byte(void)
{ 
	uint8_t temp; 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)); 
	temp = (uint8_t)SPI_I2S_ReceiveData(SPI1); 
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); 
	return temp; 
} 
