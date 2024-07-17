/*
 * myUart.h
 *
 *  Created on: Jul 11, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_MYUART_H_
#define MYDRIVERS_INC_MYUART_H_

#include "stm32f4xx_hal.h"

class Uart {
public:
    Uart(USART_TypeDef *pGirilenUart);
    void Yapilandir(uint32_t baudRate, GPIO_TypeDef *txPort, uint16_t txPin, GPIO_TypeDef *rxPort, uint16_t rxPin);
    void transmit(uint8_t *data, uint16_t size);
    void receive(uint8_t *buffer, uint16_t size);
    void receiveIT(uint8_t *buffer, uint16_t size);
    void IRQHandler();

private:
    USART_TypeDef *pGirilenUart;
    UART_HandleTypeDef huart;
    GPIO_InitTypeDef GPIO_InitStruct;
};




#endif /* MYDRIVERS_INC_MYUART_H_ */
