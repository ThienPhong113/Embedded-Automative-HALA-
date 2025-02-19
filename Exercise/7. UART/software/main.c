#include "main.h"

int main()
{
	//int i;
	char data[] = {'P', 'H', 'O', 'N' , 'G'};
	
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	UART_Init(9600, NONE);
	while(1)
	{
		UART_Transmitt('a');
		delay_ms(1000);
	}
}   

void UART_Init(unsigned int baud, unsigned int parity)
{
	parity_mode = parity;
	UART_Cycle = (72000000 / baud);
	TX_1;
}
   
uint8_t UART_CalculateParity(char cmd)
{
  uint8_t sum = 0;
	for(int i = 0; i < 8; i++)
	{
		sum += (cmd >> i) & 0x01;
	}
	if(parity_mode == EVEN)  return (sum % 2 == 0) ? 0 : 1;
	else if(parity_mode == ODD)	return (sum % 2 == 0) ? 1 : 0;
}

void UART_Transmitt(char cmd)
{
	//Processor (calculate parity bit)
	uint8_t parity_bit = UART_CalculateParity(cmd);
	//Begin transfer data proccess
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1); 
	//(1) Start Condition
	TX_0;
	delay_us(UART_Cycle);
	
	//(2) Send 8 bit
	for(int i = 0; i < 8; i++)
	{
		//if((cmd >> i) & 0x01) TX_1;
		(cmd & (1 << i))? TX_1	:	TX_0;
		delay_us(UART_Cycle);
	}
	//(3) Send Parity bit
	if(parity_mode != NONE)
	{
		if(parity_bit) TX_1;
		else TX_0;
		delay_us(UART_Cycle);
	}
	//(4) Send Stop bit
	TX_1; //Return idle state
	delay_us(UART_Cycle);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 0);
}

char UART_Receive()
{
	int i;
	uint8_t data = 0;
	uint8_t parity;
	//(1) Wait start condition
	while(GPIO_ReadInputDataBit(UART_GPIO, RX_Pin));
	delay_us(1.5 * UART_Cycle);
	
	//(2) Read data
	for(i = 0; i < 8; i++)
	{
		data |= (GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) << i);
		delay_us(UART_Cycle);
	}
	//(3) Read parity bit
	if(parity_mode != NONE)
	{
		parity =  GPIO_ReadInputDataBit(UART_GPIO, RX_Pin);
		delay_us(UART_Cycle);
	}
	//(4) Read Stop bit
	if(!GPIO_ReadInputDataBit(UART_GPIO, RX_Pin)) return 0;
	else
	{
		//Check parity bit
		if(parity_mode != NONE)
		{
			uint8_t parityCheck = UART_CalculateParity(data);
			if(parityCheck != parity) return -1;
			else return data;
		}
		else return data;
	}
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

void RCC_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
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
	GPIO_InitTypeDef GPIOInitStruct;
	
	GPIOInitStruct.GPIO_Pin = TX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = RX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIOInitStruct);
}


