
#include <msp430g2553.h>

#define CHEIO 1
#define VAZIO 2

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= (BIT0 + BIT6);
  P1REN |= (BIT3 + BIT4);
  P1OUT |= (BIT3 + BIT4);
  
  
  unsigned char estado = CHEIO;
  for(;;){
    switch(estado){
      
    case CHEIO:
        P1OUT |= BIT6;
        P1OUT &= ~BIT0;
        if((P1IN&BIT3)==0 && (P1IN&BIT4)==BIT4){
          estado = VAZIO;
        }
        break;
      case VAZIO:
        P1OUT &= ~BIT6;
        P1OUT |= BIT0;
        if((P1IN&BIT4)==0){
          estado=CHEIO;
        }
        break;
    }
      
      
  }
}
