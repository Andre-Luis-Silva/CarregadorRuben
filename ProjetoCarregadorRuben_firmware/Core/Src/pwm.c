/*
 * pwm.c
 *
 *  Created on: May 3, 2024
 *      Author: Andre
 */

#include "pwm.h"

// TIMx_ARR - Ajuste de frequência
// TIMx_CCRx - Ajuste de duty cycle
// TIMx_CCMRx - Define o tipo de PWM
// TIMx_CR1 - Auto reload registro
// TIMx_CCER - Polaridade do pino
// TIMx_BDTR - Aciona a saída OCx
// TIMx_CNT - Contador de tempo

void PwmConfig(void)
{
	RCC->IOPENR = RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;
	RCC->APBENR2 = RCC_APBENR2_TIM1EN;	// Enable TIM1 peripheral
	TIM1->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
				  TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1M_2;
	TIM1->CCER = TIM_CCER_CC1NE | TIM_CCER_CC2NE;
	TIM1->BDTR = TIM_BDTR_MOE | TIM_BDTR_BKP | TIM_BDTR_BK2P ;
	TIM1->ARR = 65535;
	TIM1->CCR1 = 32767;
	TIM1->EGR = TIM_EGR_UG;
	TIM1->CR1 = TIM_CR1_CEN;
}
