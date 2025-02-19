#include "main.h"

int main()
{
	volatile int i;
	uint8_t DataTrans[] = {1,3,9,10,15,19,90};//Data
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	SPI_Init();
	while(1)
	{
		for(i = 0; i < 7; i++)
		{
			SPI_Master_Transmit(DataTrans[i]);
			delay_ms(1000);
		}
	}
}

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
//Thiet lap chan GPIO
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//Master	
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);	
}

//Cau hinh Timer
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	TIMER_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER_InitStructure.TIM_Period = 36000 - 1;
	TIMER_InitStructure.TIM_Prescaler = 1;
	TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIMER_InitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint32_t ms)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) <  36 * ms);
}

//Tao xung clk
void Clock()
{
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);
	delay_ms(4);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	delay_ms(4);
}

void SPI_Init()
{
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}

void SPI_Master_Transmit(uint8_t byte_out)
{
	int i; 
	uint8_t byte_in;
	//(1) Start Condition
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
	delay_ms(1);
	
	//(2)Send and Receive 8bits to Slave
	for(i = 0; i < 8; i++)
	{
		GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, (byte_out << i) & 0x80);
		byte_in = (byte_in << i) | (uint8_t)GPIO_ReadInputDataBit(SPI_GPIO, SPI_MISO_Pin);
		Clock();
	}
}

