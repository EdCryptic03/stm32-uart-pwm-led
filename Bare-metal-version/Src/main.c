#include <stdint.h>
#include <stdio.h>
#include <string.h>
// RCC Clocks
#define RCC_BASE 0x40023800
#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x40))

#define TIM2_BASE 0x40000000
#define TIM2_PSC (*(volatile uint32_t*)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile uint32_t*)(TIM2_BASE + 0x2C))
#define TIM2_CR1 (*(volatile uint32_t*)(TIM2_BASE + 0x00))
#define TIM2_CCMR1 (*(volatile uint32_t*)(TIM2_BASE + 0x18))
#define TIM2_CCER (*(volatile uint32_t*)(TIM2_BASE + 0x20))
#define TIM2_CCR2 (*(volatile uint32_t*)(TIM2_BASE + 0x38))

//GPIO
#define GPIOA_BASE 0x40020000
#define GPIOA_MODER (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL (*(volatile uint32_t*)(GPIOA_BASE + 0x20))


//USART
#define USART2_BASE 0x40004400
#define USART2_SR (*(volatile uint32_t*)(USART2_BASE + 0x00))
#define USART2_DR (*(volatile uint32_t*)(USART2_BASE + 0x04))
#define USART2_BRR (*(volatile uint32_t*)(USART2_BASE + 0x08))
#define USART2_CR1 (*(volatile uint32_t*)(USART2_BASE + 0x0C))




void TIM2_Init(void){
	RCC_AHB1ENR |= (1 << 0);
	RCC_APB1ENR |= (1 << 0);

	// Configuring PA1 for GPIOA_MODER
	GPIOA_MODER &= ~(3<<2);
	GPIOA_MODER |= (2 << 2);

	GPIOA_AFRL &= ~(0xF << 4);
	GPIOA_AFRL |= (0b0001 << 4);


	TIM2_PSC = 84 - 1;
	TIM2_ARR = 100 - 1;

	// Output Mode enable for PWM
	TIM2_CCMR1 &= ~(7 << 12);
	TIM2_CCMR1 |= (6 << 12);

	//Pre-load mode enable
	TIM2_CCMR1 |= (1 << 11);

	TIM2_CCER |= (1 << 4);

	TIM2_CR1 |= (1 << 0);

}

void USART2_Init(void){
	RCC_APB1ENR |= (1 << 17);

	//Configuring PA2 and PA3 for USART_TX and USART_RX
	GPIOA_MODER &= ~(3 << 4); // For PA2
	GPIOA_MODER |= (2 << 4);

	GPIOA_MODER &= ~(3 << 6); // For PA3
	GPIOA_MODER |= (2 << 6);


	//Mapping PA2 and PA3 for Alternate function
	GPIOA_AFRL &= ~(0b1111 << 8); // For PA2
	GPIOA_AFRL |= (0b0111 << 8);

	GPIOA_AFRL &= ~(0b1111 << 12); // For PA3
	GPIOA_AFRL |= (0b0111 << 12);

	//Enabling Transmitter and Receiver
	USART2_CR1 =  0x00000000;
	USART2_CR1 |= (1 << 3); // Enabling Transmitter
	USART2_CR1 |= (1 << 2); // Enabling Receiver
	USART2_CR1 |= (1 << 13);


	USART2_BRR = 0x88;


}

int main(void){
	uint8_t input_number = 0;
	TIM2_Init();
	USART2_Init();
	while(1){
	    uint8_t received_bytes;

	    while(!(USART2_SR & (1 << 5)));

	    received_bytes = USART2_DR;

	    if(received_bytes >= '0' && received_bytes <= '9'){
	        input_number = input_number * 10 + (received_bytes - '0');
	    }
	    else if (received_bytes == '\r' || received_bytes == '\n'){
	        if (input_number > 100) input_number = 100;
	        TIM2_CCR2 = input_number;
	        input_number = 0;


	    }
	}
	}

