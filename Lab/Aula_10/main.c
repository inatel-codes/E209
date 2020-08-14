#include "msp430g2553.h"

unsigned long int aux;
unsigned int valorLido;
unsigned int tensao;

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= BIT6;
  P1SEL |= BIT6;
  TA0CCTL1 = OUTMOD_7;
  TA0CCR0 = 20000-1;
  TA0CCR1 = 0;
  TA0CTL = TASSEL_2 + ID_0 + MC_1;

  ADC10AE0 = BIT1 + BIT2;
  ADC10CTL1 = ADC10SSEL_3;
  // VCC E GND
  ADC1OCTL0 |= SREF_0;
  // GERADOR E GND
  ADC10CTL0 |= SREF_1;
  ADC10CTL0 |= REFON;
  ADC10CTL0 |= REF2_5V;
  
  ADC10CTL0 |= (ADC10ON);
  ADC10CTL0 |= (ENC + ADC10SC);
  ADC10CTL1 = INCH_1;
  
  BIS_SR(GIE);
  
  for(;;){
    
  }
 
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC_ISR(void)
{
  for (int i = 0; i < 100; i++) {
    ADC10CTL0|= (ENC + ADC10SC);
    while ((ADC10CTL0&ADC10IFG)==0) {};
    valorLido = valorLido + ADC10MEM;
  } 
  
  valorLido = valorLido / 100;
  
  aux = valorLido*3300;
  aux = aux / 1023;
  tensao = (usigned int) aux;
}
