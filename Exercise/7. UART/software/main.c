#include "main.h"

int main()
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	UART_Init(9600, NONE);
	
	int i;
	char data[] = {'P', 'H', 'O', 'N' , 'G'};
	while(1)
	{
		for(i = 0; i < 5; i++)
		{
			UART_Transmitt(data[i]);
			delay_ms(1000);
		}
	}
}   

void UART_Init(unsigned int baud, unsigned int parity_mode)
{
	parity_mode = parity_mode;
	UART_Cycle = 1.0 / baud * 1000 * 1000;
	TX_1;
}
   
uint8_t UART_CalculateParity(char cmd)
{
	int i;
  uint8_t sum = 0;
	for(i = 0; i < 8; i++)
	{
		sum += (cmd >> i) & 0x01;
	}
	if(parity_mode == EVEN)  return (sum % 2 == 0) ? 0 : 1;
	else if(parity_mode == ODD)	return (sum % 2 == 0) ? 1 : 0;
}

void UART_Transmitt(char cmd)
{
	int i;
	//Processor (calculate parity bit)
	uint8_t parity_bit = UART_CalculateParity(cmd);
	
	//Begin transfer data proccess
	//(1) Start Condition
	TX_0;
	delay_ms(UART_Cycle);
	
	//(2) Send 8 bit
	for(i = 0; i < 8; i++)
	{
		if((cmd >> i) & 0x01) TX_1;
		else TX_0;
		delay_ms(UART_Cycle);
	}
	//(3) Send Parity bit
	if(parity_mode != NONE)
	{
		if(parity_bit) TX_1;
		else TX_0;
		delay_ms(UART_Cycle);
	}
	//(4) Send Stop bit
	TX_1; //Return idle state
	delay_ms(UART_Cycle);
}

char UART_Receive()
{
	int i;
	uint8_t data;
	uint8_t parity;
	//(1) Wait start condition
	while(GPIO_ReadInputDataBit(UART_GPIO, RX_Pin));
	delay_ms(1.5 * UART_Cycle);
	
	//(2) Read data
	for(i = 0; i < 8; i++)
	{
		data = (data << i) | GPIO_ReadInputDataBit(UART_GPIO, RX_Pin);
		delay_ms(UART_Cycle);
	}
	//(3) Read parity bit
	if(parity_mode != NONE)
	{
		parity =  GPIO_ReadInputDataBit(UART_GPIO, RX_Pin);
		delay_ms(UART_Cycle);
	}
	//(4) Read Stop bit
	if(!GPIO_ReadInputDataBit(UART_GPIO, RX_Pin)) return 0;
	else
	{
		//Check parity bit
		if(parity_mode != NONE)
		{
			uint8_t parityCheck = UART_CalculateParity(data);
			if(parityCheck != parity) return 0;
			else return data;
		}
		else return data;
	}
}

void delay_ms(uint32_t ms)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < ms);
}

void RCC_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef timer2;
	timer2.TIM_ClockDivision = TIM_CKD_DIV1;
	timer2.TIM_CounterMode = TIM_CounterMode_Up;
	timer2.TIM_Period = 7;
	timer2.TIM_Prescaler = 999;
	
	TIM_TimeBaseInit(TIM2, &timer2);
	TIM_Cmd(TIM2, ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	GPIOInitStruct.GPIO_Pin = TX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = RX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
}


