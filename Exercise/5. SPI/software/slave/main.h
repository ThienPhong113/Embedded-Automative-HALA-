#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#define SPI_SCK_Pin 	GPIO_Pin_0
#define SPI_MISO_Pin	GPIO_Pin_1
#define SPI_MOSI_Pin 	GPIO_Pin_2
#define SPI_CS_Pin 		GPIO_Pin_3
#define SPI_GPIO 			GPIOA

void RCC_Config(void);
void TIM_Config(void);
void GPIO_Config(void);

uint8_t SPI_Slave_Receive(uint8_t byte_out); 

void delay_ms(uint32_t ms);
