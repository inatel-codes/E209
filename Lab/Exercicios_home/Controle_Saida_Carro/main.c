#include "msp430g2553.h"

#define VERIFICACAO 0
#define ALERTA 1
#define PREPARO 2
#define ERRO 3

unsigned char estado = 0;
unsigned int tempo = 0;

// PARA SIMULAR
unsigned char botao = 1;
unsigned char LED_VERMELHO;
unsigned char LED_VERDE;

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
      case VERIFICACAO:
        //P1OUT = P1OUT &~ BIT0;
        //P1OUT = P1OUT &~ BIT6;
        
        LED_VERDE = 0;
        LED_VERMELHO = 0;
        
        if((botao) == 0)
        {
          estado = ALERTA;
        }
        break;
        
      case ALERTA:
        while(((botao) == 0) && tempo < 10)
        {
          //P1OUT = P1OUT | BIT6;
          //P1OUT = P1OUT &~ BIT0;
          
          LED_VERMELHO = 0;
          LED_VERDE = 1;
        
          __delay_cycles(400000);
          
          //P1OUT = P1OUT | BIT0;
          //P1OUT = P1OUT &~ BIT6;
          
          LED_VERDE = 0;
          LED_VERMELHO = 1;
          
          __delay_cycles(400000);
          
          tempo++;
        }
        
        if(tempo == 10)
        {
          tempo = 0;
          estado = ERRO;
        }
        
        else
        {
          estado = PREPARO;
          tempo = 0;
        }
        
        break;
        
      case PREPARO:
        //P1OUT = P1OUT &~ BIT0;
        //P1OUT = P1OUT | BIT6;
        
        LED_VERMELHO = 0;
        LED_VERDE = 1;
        
        __delay_cycles(4000000);
        
        //P1OUT = P1OUT &~ BIT6;
        LED_VERDE = 0;
        
        __delay_cycles(250000);
        
        //P1OUT = P1OUT | BIT6;
        LED_VERDE = 1;
        
        estado = VERIFICACAO;
        
        break;
        
      case ERRO:
        //P1OUT = P1OUT | BIT0;
        //P1OUT = P1OUT &~ BIT6;
        
        LED_VERDE = 0;
        LED_VERMELHO = 1;
        
        if((botao) != 0)
        {
          estado = PREPARO;
        }
        break;
    }
  }

}
