/*
 * adcFile.c
 *
 *  Created on: May 2, 2024
 *      Author: PDS-User
 */
#include "adcFile.h"

void ConfigAdc(void)
{
	RCC->APBENR2 = RCC_APBENR2_ADCEN;
	ADC1->CR = ADC_CR_ADVREGEN;
	for(unsigned int i = 0; i < 65535; i++);
	ADC1->CR |= ADC_CR_ADCAL;
	while(ADC1->CR & ADC_CR_ADCAL);
	ADC1->CR |= ADC_CR_ADEN;
	ChannelConfigurationReadyFlag();
	ADC1->CFGR1 &= ~ADC_CFGR1_CHSELRMOD;
	ChannelConfigurationReadyFlag();

}


void ChannelConfigurationReadyFlag(void)
{
	while(!(ADC1->ISR & ADC_ISR_ADRDY));
}

int ReadAdc(int channel)
{
	ADC1->CHSELR = (1 << channel);
	ChannelConfigurationReadyFlag();
	ADC1->CR |= ADC_CR_ADSTART;
	while(ADC1->CR & ADC_CR_ADSTART);
	ADC1->CHSELR = 0;
	return ADC1->DR;
}
