#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#define SPI1_NSS GPIO_Pin_4
#define SPI1_SCK GPIO_Pin_5
#define SPI1_MISO GPIO_Pin_6
#define SPI1_MOSI GPIO_Pin_7
#define SPI1_GPIO GPIOA

void RCC_Config(void);
void TIM_Config(void);
void GPIO_Config(void);
void SPI1_Config(void);

void delay_ms(uint32_t ms);

void SPI_Send1Byte(uint8_t data);
uint8_t SPI_Receive1Byte(void);

uint8_t dataSend[] = {3, 1, 10, 19, 20, 36, 90};
