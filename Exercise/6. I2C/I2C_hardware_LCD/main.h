#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#define I2C_SCL   GPIO_Pin_6
#define I2C_SDA	  GPIO_Pin_7
#define I2C1_GPIO GPIOB

uint8_t slave_address;

void RCC_Config(void);
void TIM_Config(void);
void I2C_Config(void);
void GPIO_Config(void);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

void I2C_Write(uint8_t addr, uint8_t byte);

void lcd_init(uint8_t addr);
void lcd_send_cmd (char cmd); 
void lcd_send_data (char data);
void lcd_send_string (char *str);
void lcd_clear_display (void);	
void lcd_goto_XY (int row, int col);