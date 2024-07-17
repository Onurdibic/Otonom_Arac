/*
 * myUart.cpp
 *
 *  Created on: Jul 11, 2024
 *      Author: onurd
 */
#include "myUart.h"

Uart::Uart(USART_TypeDef *pGirilenUart)
{
	this->pGirilenUart=pGirilenUart;
}

void Uart::Yapilandir(uint32_t baudRate, GPIO_TypeDef *txPort, uint16_t txPin, GPIO_TypeDef *rxPort, uint16_t rxPin) {

    GPIO_InitStruct.Pin = txPin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(txPort, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = rxPin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(rxPort, &GPIO_InitStruct);


    huart.Instance = pGirilenUart;
    huart.Init.BaudRate = baudRate;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart) != HAL_OK)
    {
    	while(1)
    		{

    	    }
    }
}

void Uart::transmit(uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit(&huart, data, size, HAL_MAX_DELAY);
}
void Uart::receive(uint8_t *buffer, uint16_t size)
{
    HAL_UART_Receive(&huart, buffer, size, HAL_MAX_DELAY);
}
void Uart::receiveIT(uint8_t *buffer, uint16_t size)
{
    HAL_UART_Receive_IT(&huart, buffer, size);
}

