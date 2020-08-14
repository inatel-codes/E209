#include "msp430g2553.h"

#define REFRIGERACAO_OFF 0
#define REFRIGERACAO_ON 1
#define NORMAL 0
#define MAXIMA 1
#define ALERTA 2

unsigned char estado = 0;
unsigned char temperatura;
unsigned char nao_resfriou = 0;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= (BIT0+BIT6);
  P1REN |= BIT3;
  P1OUT |= BIT3;
  
  while(1)
  {
    switch(estado)
    {
      case REFRIGERACAO_OFF:
        P1OUT = P1OUT &~ BIT0;
        P1OUT = P1OUT &~ BIT6;
        
        if(temperatura == MAXIMA)
        {
          estado = REFRIGERACAO_ON;
        }
        break;
        
      case REFRIGERACAO_ON:
        P1OUT = P1OUT | BIT0;
        P1OUT = P1OUT &~ BIT6;
        
        if(temperatura == NORMAL)
        {
          estado = REFRIGERACAO_OFF;
        }
        
        __delay_cycles(500000);
        nao_resfriou++;
        
        if(nao_resfriou == 3)
        {
          estado = ALERTA;
        }
        break;
       
      case ALERTA:
        P1OUT = P1OUT | BIT6;
        nao_resfriou = 0;
        
        if(temperatura == NORMAL)
        {
          estado = REFRIGERACAO_OFF;
        }
        break;
        
      default: break;
    }
  }
}
