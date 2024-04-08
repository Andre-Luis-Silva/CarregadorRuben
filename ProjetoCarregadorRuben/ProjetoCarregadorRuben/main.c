#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define FACTOR_VOLTAGE	0.0118	// It's the factor to convert AD measure in Voltage: 1.1*11/1024
#define INPUT_VOLTAGE	0
#define OUTPUT_VOLTAGE	1
#define BATTERY_VOLTAGE	2
#define MAXIMUM_PWM	255

void ConfigADC(void);
void ConfigGPIO(void);
void ConfigPWM(void);
int ReadAdc(int channel);

int main (void) 
{

  ConfigGPIO();
  ConfigADC();
  ConfigPWM();
  //Serial.begin(115200);
  OCR1A = 239;
  
	uint8_t pwmBoost = MAXIMUM_PWM;
	uint8_t pwmBuck = MAXIMUM_PWM;
	OCR1A = MAXIMUM_PWM;
	OCR1B = MAXIMUM_PWM;
	float voltage12V = 0;
	float voltage3dot7V = 0;
	uint8_t flagLowVoltage = 0;
	uint8_t estado = 0;
	float outputVoltage = 0;
	int outputVoltageBuck = 0;
	for(;;) 
	{
		voltage12V = ReadAdc(INPUT_VOLTAGE) * FACTOR_VOLTAGE;
		switch(estado)
		{
			case 0:
			
				if(voltage12V < 7)
				{
					estado = 1;
					OCR1A = MAXIMUM_PWM;
				}
				else if(voltage12V > 7.3)
				{
					estado = 2;
					OCR1B = MAXIMUM_PWM;
				}
				
			break;

			case 1:
			
				PORTD &= ~(1 << PD2);
				voltage3dot7V = ReadAdc(BATTERY_VOLTAGE) * FACTOR_VOLTAGE;
				if(voltage3dot7V > 3.2)
				{
					outputVoltage = ReadAdc(OUTPUT_VOLTAGE) * FACTOR_VOLTAGE;
					if((outputVoltage > 5) && pwmBoost < 255)
					{
						pwmBoost++;
					}
					else if((outputVoltage < 5) && pwmBoost > 0)
					{
						pwmBoost--;
					}
					OCR1B = pwmBoost;
				}
				else
				{
					OCR1B = MAXIMUM_PWM;
				}

				if(voltage12V > 7.3)
				{
					estado = 2;
					OCR1B = MAXIMUM_PWM;
				}

			break;

			case 2:
				
				PORTD |= (1 << PD2);
				outputVoltageBuck = ReadAdc(OUTPUT_VOLTAGE) * FACTOR_VOLTAGE;
				if((outputVoltageBuck > 5) && pwmBuck > 0)
				{
					pwmBuck--;
				}
				else if((outputVoltageBuck < 5) && pwmBuck < 255)
				{
					pwmBuck++;
				}
				OCR1A = pwmBuck;
				if(voltage12V < 7)
				{
					estado = 1;
					OCR1A = MAXIMUM_PWM;
				}

			break;

		}

	}
}

void ConfigADC(void)
{
  /* 
  The ADC was configured using the 1.1 internal regulator to make 
  the measure of the voltage of battery 
  */
  ADMUX = (1 << REFS1) | (1 << REFS0);
  ADCSRA = (1 << ADEN);
}

void ConfigGPIO(void)
{
  /* The Output pins are PB1, PB2 and PD2 */
  DDRB |= (1 << PB1) | (1 << PB2);
  DDRD |= (1 << PD2);
}

void ConfigPWM(void)
{
  /* 
  The Timer1 was configured to Fast PWM, without prescaler
  and a frequency in 62.5 kHz.
  */
  TCCR1A |= (1 << COM1B1) | (1 << COM1A1) | (1 << WGM10); 
  TCCR1B |= (1 << CS10) | (1 << WGM12);
}

int ReadAdc(int channel)
{
  ADMUX &= (0xF0);	
  ADMUX |= (channel); // Read the value of the channel
  ADCSRA |= (1 << ADSC);  // Start the conversion
  while(ADCSRA & (1 << ADSC));  // Wait the flag of finish conversion
  return ADC; // Return the value of ADC
}
