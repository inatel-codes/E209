#include "msp430g2553.h"

#define DESLIGADO 0
#define LIGADO 1

unsigned char estado = 0;
unsigned int tempo = 0;

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = P1DIR | (BIT0 + BIT6);
  P1REN = P1REN | BIT3;
  P1OUT = P1OUT | BIT3;
  
  for(;;)
  {
    switch(estado)
    {
      case DESLIGADO:
        P1OUT = P1OUT | BIT0;
        P1OUT = P1OUT &~ BIT6;
        
        while(((P1IN&BIT3) == 0) && tempo < 500)
        {
          __delay_cycles(10000);
          tempo++;
        }
        
        if(tempo == 500)
          estado = LIGADO;
        
        else
          tempo = 0;
        break;
        
      case LIGADO:
        P1OUT = P1OUT &~ BIT0;
        P1OUT = P1OUT | BIT6;
        __delay_cycles(10000000);
        tempo = 0;
        estado = DESLIGADO;
        break;
        
      default: 
        break;
      
    }
  }


}
