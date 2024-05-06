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
