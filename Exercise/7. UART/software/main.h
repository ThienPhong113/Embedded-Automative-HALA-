#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#define TX_Pin GPIO_Pin_0
#define RX_Pin GPIO_Pin_1
#define UART_GPIO GPIOA

#define TX_1	GPIO_SetBits(UART_GPIO, TX_Pin)
#define TX_0	GPIO_ResetBits(UART_GPIO, TX_Pin)
#define RX_1	GPIO_SetBits(UART_GPIO, RX_Pin)
#define RX_0	GPIO_ResetBits(UART_GPIO, RX_Pin)

unsigned int UART_Cycle;
unsigned int parity_mode;
	 
void RCC_Config();
void TIM_Config();
void GPIO_Config();

void delay_ms(uint32_t ms);

void UART_Init(unsigned int baud, unsigned int parity_mode);
uint8_t UART_CalculateParity(char cmd);
void UART_Transmitt(char cmd);
char UART_Receive();

enum parity_mode
{
	NONE,
	ODD,
	EVEN
};