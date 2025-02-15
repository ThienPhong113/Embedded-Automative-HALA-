#include "main.h"

int main()
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	UART_Config();
	while(1)
	{
		
	}
}


void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 7;
	timer.TIM_Prescaler = 999;
	
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_Cmd(TIM2, ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = RX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = TX_Pin;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
}
void UART_Config()
{
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}
void USART1_TransmitByte(uint8_t byte) 
{
  // Wait until the transmit data register is empty (TXE flag is set)
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  // Transmit the byte
  USART_SendData(USART1, byte);
  // Wait until transmission is complete (TC flag is set)
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
uint8_t USART1_ReceiveByte(void)
{
	uint8_t temp = 0x00;
	// Wait until data is received (RXNE flag is set)
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  // Read the received data
	temp = USART_ReceiveData(USART1);
	return temp;
}
void delay_ms(uint32_t ms)
{
	
}