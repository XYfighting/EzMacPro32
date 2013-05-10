#ifndef _HARDWARE_DEFS_H_
#define _HARDWARE_DEFS_H_

#include "stm32f10x.h"
#include "bsp_gpio.h"

/*!
 * System Clock and frequency divider definition.
 */
#define SYSCLK_HZ				(72000000L)
#define MAC_TIMER_PRESCALER		72L
#define DELAY_TIMER_PRESCALER	72L

#define MAC_TIMER				TIM3
#define MAC_TIMER_CLOCK()		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE )
#define MAC_TIMER_IRQn			TIM3_IRQn
#define timerIntT3_ISR			TIM3_IRQHandler

#define DELAY_TIMER				TIM2
#define DELAY_TIMER_CLOCK()		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE )
#define delayIntT2_ISR			TIM2_IRQHandler

#define LED1_PIN			PB12
#define LED2_PIN			PB11

#define RF_SDN_PIN			PA2
#define RF_NSS_PIN			PA4
#define RF_SCLK_PIN			PA5
#define RF_MISO_PIN			PA6
#define RF_MOSI_PIN			PA7

#define RF_IRQ_PIN			PA3
#define RF_IRQ_EXT_PORT		GPIO_PortSourceGPIOA
#define RF_IRQ_EXT_PIN		GPIO_PinSource3
#define RF_IRQ_EXT_LINE		EXTI_Line3
#define RF_IRQ_EXT_IRQ		EXTI3_IRQn
#define externalIntISR		EXTI3_IRQHandler
	
#define RF_SCLK_INIT()		pinMode ( RF_SCLK_PIN, MODE_AltPushPull_50M )
#define RF_MOSI_INIT()		pinMode ( RF_MOSI_PIN, MODE_AltPushPull_50M )
#define RF_MISO_INIT()		pinMode ( RF_MISO_PIN, MODE_Input )

#define RF_SDN_LOW()		pinLow  ( RF_SDN_PIN )
#define RF_SDN_HIGH()		pinHigh ( RF_SDN_PIN )
#define RF_SDN_INIT()		pinMode ( RF_SDN_PIN, MODE_PushPull_2M)

// pinLow  ( RF_NSS_PIN )
// pinHigh ( RF_NSS_PIN )
#define RF_NSS_LOW()		GPIOA->BRR  = (1 << (RF_NSS_PIN & 0x0F))
#define RF_NSS_HIGH()		GPIOA->BSRR = (1 << (RF_NSS_PIN & 0x0F))
#define RF_NSS_INIT()		pinMode ( RF_NSS_PIN, MODE_PushPull_50M )

#define RF_IRQ_READ()		(GPIOA->IDR & (1 << (RF_IRQ_PIN & 0x0F)))
#define RF_IRQ_INIT()		pinMode ( RF_IRQ_PIN, MODE_Input )

#define RF_SPI				SPI1
#define RF_SPI_CLOCK()		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)

#define LED1_OFF()			pinLow  ( LED1_PIN )
#define LED2_OFF()			pinLow  ( LED2_PIN )
#define LED1_ON()			pinHigh ( LED1_PIN )
#define LED2_ON()			pinHigh ( LED2_PIN )
#define LED1_INIT()			pinMode ( LED1_PIN, MODE_PushPull_2M)
#define LED2_INIT()			pinMode ( LED2_PIN, MODE_PushPull_2M )
#define LED2_READ()			(PIN_PORT(LED2_PIN)->ODR & PIN_MASK(LED2_PIN))
#define LED2_SET(s)			if (s) LED2_ON(); else LED2_OFF()

/*!
 * Interrupt macros.
 */
#define ENABLE_GLOBAL_INTERRUPTS()      __enable_irq()
#define DISABLE_GLOBAL_INTERRUPTS()     __disable_irq()

#define ENABLE_MAC_EXT_INTERRUPT()		EXTI->IMR |=  RF_IRQ_EXT_LINE
#define CLEAR_MAC_EXT_INTERRUPT()		EXTI->PR   =  RF_IRQ_EXT_LINE
#define GET_MAC_EXT_INTERRUPT()			((EXTI->IMR & RF_IRQ_EXT_LINE) ? 1 : 0)
#define DISABLE_MAC_EXT_INTERRUPT_INT()	\
	do {								\
		EXTI->IMR &= ~RF_IRQ_EXT_LINE;	\
		CLEAR_MAC_EXT_INTERRUPT();		\
	} while (0)
#define DISABLE_MAC_EXT_INTERRUPT()		\
	do {								\
		DISABLE_GLOBAL_INTERRUPTS();	\
		DISABLE_MAC_EXT_INTERRUPT_INT();\
		ENABLE_GLOBAL_INTERRUPTS();		\
	} while (0)
#define SET_MAC_EXT_INTERRUPT(enable)	\
	do {								\
		if (enable)						\
			ENABLE_MAC_EXT_INTERRUPT();	\
	} while (0)

#define ENABLE_MAC_INTERRUPTS()			\
	do {								\
		ENABLE_MAC_EXT_INTERRUPT();		\
		ENABLE_MAC_TIMER_INTERRUPT();	\
	} while (0)
#define DISABLE_MAC_INTERRUPTS()		\
	do {									\
		DISABLE_GLOBAL_INTERRUPTS();		\
		DISABLE_MAC_EXT_INTERRUPT_INT();	\
		DISABLE_MAC_TIMER_INTERRUPT_INT();	\
		ENABLE_GLOBAL_INTERRUPTS();			\
	} while (0)

#define ENABLE_UART_INTERRUPT()         ENABLE_UART0_INTERRUPT()
#define SET_UART_INTERRUPT_FLAG()       SET_UART0_INTERRUPT_FLAG()
#define CLEAR_UART_INTERRUPT_FLAG()     CLEAR_UART0_INTERRUPT_FLAG()

/*!
 * UART baud rate.
 */
#define UART0_BAUDRATE                   (115200L)

#define DISABLE_WATCHDOG()

#endif //_HARDWARE_DEFS_H_