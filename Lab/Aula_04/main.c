#include "msp430g2553.h"

#define TEMP_ON 5000  // Tempo que a saida ficara ligada em ms
int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  P1DIR = P1DIR | BIT0; // P1.0 = Saida
  P1DIR = P1DIR | BIT6;
  //P1REN = P1REN | BIT3; // ligando resistor interno do P1.3
  P1OUT = P1OUT | BIT3; // ativando o resistor como pull-up
  P1OUT = P1OUT &(~BIT6);
  P1OUT = P1OUT &(~BIT0); // desliga a saida
  
  for(;;)
  {
    if((P1IN&BIT3) == 0) // botao pressionado?
    {
      P1OUT = P1OUT | BIT6; // aciona a saida
      P1OUT = P1OUT & (~BIT0);
      __delay_cycles(500000);
      P1OUT = P1OUT | BIT0;
      P1OUT = P1OUT & (~BIT6);
      __delay_cycles(500000);
      //P1OUT = P1OUT & (~BIT6); // desliga a saida
      //P1OUT = P1OUT & (~BIT0);
    }
  }
}

