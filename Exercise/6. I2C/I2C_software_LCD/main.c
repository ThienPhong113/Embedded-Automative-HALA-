#include "main.h"

int main()
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	I2C_Init();
	lcd_init(0x27);
	while(1)
	{
		//lcd_send_string("ThienPhong");
		lcd_send_data('a');
		delay_ms(500);
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}
void TIM_Config(void)
{
//36000 72
	TIM_TimeBaseInitTypeDef timer;
	
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 0xFFFF;
	timer.TIM_Prescaler = 36000 - 1;
	TIM_TimeBaseInit(TIM2, &timer);
	
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 0xFFFF;
	timer.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM3, &timer);
	
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SDA| I2C_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

void delay_ms(uint32_t ms)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < 2 * ms);
}
void delay_us(uint32_t us)
{
	TIM_SetCounter(TIM3, 0);
	while(TIM_GetCounter(TIM3) < us);
}

void I2C_Init(void)
{
	SDA_1;
	SCL_1;
}
void I2C_Start(void)
{
	SDA_0;
	delay_us(I2C_Cycle);
	SCL_0;
	delay_us(I2C_Cycle);
}
void I2C_setAddress(uint8_t addr)
{
	for(int i = 0; i < 7; i++)
	{
		if(addr & (0x80 >> i)) SDA_1;
		else SDA_0;
		SCL_1;
		delay_us(0.5 * I2C_Cycle);
		SCL_0;
		delay_us(0.5 * I2C_Cycle);
	}
}
void I2C_selectMode(bool CS)
{
	//CS: 0 - Write   1 - Read
	GPIO_WriteBit(I2C_GPIO, I2C_SDA, CS);
	SCL_1;
	delay_us(0.5 * I2C_Cycle);
	SCL_0;
	delay_us(0.5 * I2C_Cycle);
}
bool bitACK(void)
{
  bool ACK;
  delay_us(0.5 * I2C_Cycle);
  SCL_1;
  ACK = GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA);
  delay_us(0.5 * I2C_Cycle);
	SCL_0;
  return ACK;
}
void I2C_sendByte(uint8_t byte)
{
	for(int i = 0; i < 8; i++)
	{
		if(byte & (0x80 >> i)) SDA_1;
		else SDA_0;		
		SCL_1;
		delay_us(0.5 * I2C_Cycle);
		SCL_0;
		delay_us(0.5 * I2C_Cycle);
	}
}
void I2C_Write(uint8_t address, uint8_t byte_out)
{
	I2C_Start();
	I2C_setAddress(address);
	I2C_selectMode(0);
	if(bitACK())
	{
		I2C_Stop();
		return;
	}
	else
	{
		I2C_sendByte(byte_out);
		I2C_Stop();
	}
}

uint8_t I2C_ReadByte(uint8_t address, uint8_t resAddress)
{
	I2C_Start();
	I2C_setAddress(address);
	I2C_selectMode(0);
	if(bitACK())
	{
		I2C_Stop();
		return 0;
	}
	I2C_sendByte(resAddress);
	if(bitACK())
	{
		I2C_Stop();
		return 0;
	}
	I2C_Start();
	I2C_setAddress(address);
	I2C_selectMode(1);
	if(bitACK())
	{
		I2C_Stop();
		return 0;
	}	
}


void I2C_Stop(void)
{
	SCL_1;
	delay_us(I2C_Cycle);
	SDA_1;
	delay_us(I2C_Cycle);
}

void lcd_init(uint8_t addr)
{
	slave_address = addr;
	lcd_send_cmd (0x33); 
	lcd_send_cmd (0x32);
	delay_ms(50);
	lcd_send_cmd (0x28); 
	delay_ms(50);
	lcd_send_cmd (0x01); 
	delay_ms(50);
	lcd_send_cmd (0x06); 
	delay_ms(50);
	lcd_send_cmd (0x0c); 
	delay_ms(50);
	lcd_send_cmd (0x02); 
	delay_ms(50);
	lcd_send_cmd (0x80);
}
void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	I2C_Write(slave_address, cmd);
}
void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	I2C_Write(slave_address, data);
}
void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}
void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); //clear display
}
void lcd_goto_XY (int row, int col)
{
	uint8_t pos_Addr;
	if(row == 1) 
	{
		pos_Addr = 0x80 + row - 1 + col;
	}
	else
	{
		pos_Addr = 0x80 | (0x40 + col);
	}
	lcd_send_cmd(pos_Addr);
}
