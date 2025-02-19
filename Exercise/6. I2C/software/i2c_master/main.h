#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO


#define I2C_SCL 	GPIO_Pin_0
#define I2C_SDA		GPIO_Pin_1
#define I2C_GPIO	GPIOA

//Standard Mode: speed = 100kbit/s
unsigned int I2C_Cycle =  10; //us

#define SDA_1		GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define SDA_0		GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define SCL_1		GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define SCL_0 	GPIO_ResetBits(I2C_GPIO, I2C_SCL)

void RCC_Config();
void TIM_Config();
void GPIO_Config();

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

void I2C_Init();
void I2C_Start();
void I2C_setAddress(uint8_t addr);

void I2C_sendByte(uint8_t byte);

void I2C_Stop();