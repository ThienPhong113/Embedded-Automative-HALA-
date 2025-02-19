#include "stm32f10x.h"

// Ð?nh nghia chân SPI1
#define SPI_GPIO        GPIOA
#define SPI_GPIO_CLK    RCC_APB2Periph_GPIOA
#define SPI1_CLK        RCC_APB2Periph_SPI1
#define SPI_NSS_PIN     GPIO_Pin_4   // CS
#define SPI_SCK_PIN     GPIO_Pin_5   // SCK
#define SPI_MISO_PIN    GPIO_Pin_6   // MISO
#define SPI_MOSI_PIN    GPIO_Pin_7   // MOSI

void SPI_Config(void);
uint8_t SPI_Transmit(uint8_t data);

int main(void)
{
    uint8_t dataSend = 0x55;  // D? li?u c?n g?i
    uint8_t receivedData;
    
    SPI_Config();

    while (1)
    {
        GPIO_ResetBits(SPI_GPIO, SPI_NSS_PIN); // Kéo CS xu?ng th?p (B?t d?u giao ti?p)
        receivedData = SPI_Transmit(dataSend); // G?i và nh?n d? li?u qua SPI
        GPIO_SetBits(SPI_GPIO, SPI_NSS_PIN);   // Kéo CS lên cao (K?t thúc giao ti?p)

        for (int i = 0; i < 100000; i++);  // Delay don gi?n
    }
}

// C?u hình SPI1
void SPI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef SPI_InitStruct;

    // B?t clock cho GPIO và SPI1
    RCC_APB2PeriphClockCmd(SPI_GPIO_CLK | SPI1_CLK, ENABLE);

    // C?u hình chân NSS (CS)
    GPIO_InitStruct.GPIO_Pin = SPI_NSS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
    GPIO_SetBits(SPI_GPIO, SPI_NSS_PIN); // M?c d?nh NSS ? m?c cao

    // C?u hình chân SCK, MOSI
    GPIO_InitStruct.GPIO_Pin = SPI_SCK_PIN | SPI_MOSI_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);

    // C?u hình chân MISO
    GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);

    // C?u hình SPI1
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStruct);

    // Kích ho?t SPI1
    SPI_Cmd(SPI1, ENABLE);
}

// Hàm g?i và nh?n 1 byte qua SPI
uint8_t SPI_Transmit(uint8_t data)
{
    // Ð?i d?n khi SPI1 s?n sàng d? g?i
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);

    // Ð?i d?n khi nh?n du?c d? li?u
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}
