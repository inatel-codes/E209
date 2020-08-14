#include "msp430g2553.h"

unsigned char cont = 0;
unsigned char cont1 = 0;

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1REN |= BIT3;
  P1OUT |= BIT3;
  P1DIR |= BIT6;
  P1SEL |= BIT6;
  P1DIR |= BIT0;
  
  
  P1IE = P1IE | BIT3;
  P1IES |= BIT3;
  P1IFG &=~ BIT3;
  
  TA0CCTL1 = OUTMOD_7;
  TA0CCR0 = 20000-1;
  TA0CCR1 = 0;
  TA0CTL = TASSEL_2 + ID_0 + MC_1;
  
  TA0CCTL0 &= ~CCIE;
  
  _BIS_SR(GIE);
  
  LPM1;
  
  for(;;){
  
  }
}


#pragma vector = PORT1_VECTOR
__interrupt void Button_ISR( void )
{
  __delay_cycles(200000);
  if(cont1 == 1){
    TA0CCTL0 &= ~CCIE;
    TA0CCR1 = 0;
    cont1 = 0;
  }
  else{
    TA0CCTL0 = CCIE;
  }
  P1IFG &= ~BIT3;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR( void )
{
  /*cont++;
  if(cont == 50){
    TA0CCR1 += 2000;
    if(TA0CCR1 >= 19999){
      TA0CCR1 = 19999;
      cont1 = 1;
    }
    cont = 0;
  }*/
  
  TA0CCR1 = 19999;
  cont1 = 1;
  TA0CCTL0 &=~ CCIFG;
}

