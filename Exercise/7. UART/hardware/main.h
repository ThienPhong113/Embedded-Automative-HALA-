#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

#define TX_Pin	GPIO_Pin_9
#define RX_Pin	GPIO_Pin_10
#define UART_GPIO GPIOA

void RCC_Config();
void TIM_Config();
void GPIO_Config();
void UART_Config();
void USART1_TransmitByte(uint8_t byte);
uint8_t USART1_ReceiveByte(void);

void delay_ms(uint32_t ms);