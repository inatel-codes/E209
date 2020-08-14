#include "msp430g2553.h"

unsigned char botao = 0; // simular o botao: (P1IN&BIT3) == 0
unsigned char pressionado_antes = 0;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR = P1DIR | BIT6;
  P1DIR = P1DIR | BIT0;
  P1REN = P1REN | BIT3;
  P1OUT = P1OUT | BIT3;
  P1OUT = P1OUT | BIT6;
  P1OUT = P1OUT &~ BIT0;
  
  for(;;)
  {
    if(botao == 1)
    {
      if(pressionado_antes == 0)
      {
        P1OUT = P1OUT &~ BIT6;
        P1OUT = P1OUT | BIT0;
        pressionado_antes = 1;
      }
      
      else
      {
        P1OUT = P1OUT &~ BIT6;
        P1OUT = P1OUT | BIT0;
        pressionado_antes = 0;
      }
    }
  }
  
}
