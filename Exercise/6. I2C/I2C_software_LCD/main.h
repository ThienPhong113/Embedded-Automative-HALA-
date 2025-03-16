#include "stdbool.h"
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#define I2C_SCL 	GPIO_Pin_6
#define I2C_SDA 	GPIO_Pin_7
#define I2C_GPIO	GPIOB

#define SDA_0 GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define SDA_1	GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define SCL_0	GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define SCL_1	GPIO_SetBits(I2C_GPIO, I2C_SCL)

unsigned int I2C_Cycle = 10; //(us) - Standard mode: 100Kbit/s 
uint8_t slave_address;

void RCC_Config(void);
void TIM_Config(void);
void GPIO_Config(void);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

void I2C_Init(void);
void I2C_Start(void);
void I2C_setAddress(uint8_t addr);
void I2C_selectMode(bool CS);
bool bitACK(void);
void I2C_sendByte(uint8_t byte);
uint8_t I2C_readByte(void);
void I2C_Stop(void);

void I2C_Write(uint8_t address, uint8_t byte_out);
void I2C_WriteMultiBye();
uint8_t I2C_ReadByte(uint8_t address, uint8_t resAddress);
uint8_t I2C_ReadMultiByte();

void lcd_init(uint8_t addr);
void lcd_send_cmd (char cmd); 
void lcd_send_data (char data);
void lcd_send_string (char *str);
void lcd_clear_display (void);	
void lcd_goto_XY (int row, int col);