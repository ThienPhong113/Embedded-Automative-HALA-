#include "main.h".h

//Cau hinh GPIO
//Cap xung CLK 
void RCC_Config()
{
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
}
//Thiet lap chan GPIO
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//Master	
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);	
}

void SPI_Slave_Transmit(uint8_t byte_out)
{
	uint8_t byte_in;
	//(1) waite start condition
	while(GPIO_ReadInputDataBit(SPI_GPIO,  SPI_CS_Pin));
	//(2) Wait data from Master
	for(int i = 0; i < 8; i++)
	{
		byte_in = (byte_in << i) | GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
		GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, (byte_out << i) & 0x80);
		while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
		while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	}
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
}

int main()
{
	uint8_t DataTrans[] = {1,3,9,10,15,19,90};//Data
	RCC_Config();
	GPIO_Config();
	while(1)
	{
		for(int i = 0; i < 8; i++)
		{
			SPI_Slave_Transmit(DataTrans[i]);
		}
	}
}