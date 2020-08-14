#include "msp430g2553.h"

#define TOTALMENTE_FECHADO 0
#define TOTALMENTE_ABERTO 1
#define ABRINDO 3
#define FECHANDO 4
#define PARADO_APOS_ABRINDO 5
#define PARADO_APOS_FECHANDO 6

unsigned char estado = 0; // Variavel para guardar o estado do portao
unsigned char SF = 1; // Variavel para simular o sensor que verifica se o portao está totalmente fechado
unsigned char SA = 0; // Variavel para simular o sensor que verifica se o portao está totalmente aberto
unsigned char botao = 1; // Variavel para simular o botao

void main( void )
{
  // Desabiliando o Watchdog
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR = P1DIR | (BIT0 + BIT6); // Definindo o BIT0 (Led Vermelho) e o BIT6 (Led Verde) como saida
  P1REN = P1REN | BIT3; // Habilitando o resistor interno do botao
  P1OUT = P1OUT | BIT3; // Definindo o resistor como Pull-Up (logica OU)
  
  for(;;)
  {
    switch(estado)
    {
      case TOTALMENTE_FECHADO:
        P1OUT = P1OUT &~ BIT0; // Desliga o Led Vermelho
        P1OUT = P1OUT &~ BIT6; // Desliga o Led Verde
        
        if(botao == 0)
        {
          __delay_cycles(500000);
          estado = ABRINDO;
        }
        break;
        
      case TOTALMENTE_ABERTO:
        P1OUT = P1OUT &~ BIT0; // Desliga o Led Vermelho
        P1OUT = P1OUT &~ BIT6; // Desliga o Led Verde
        
        if(botao == 0)
        {
          __delay_cycles(500000);
          estado = FECHANDO;
        }
        break;
        
      case ABRINDO:
        P1OUT = P1OUT &~ BIT0; // Desliga o Led Vermelho
        P1OUT = P1OUT | BIT6;  // Liga o Led Verde
        
        if(botao == 0)
        {
          __delay_cycles(500000);
          estado = PARADO_APOS_ABRINDO;
        }
        
        if(SA == 1 && SF == 0)
        {
          estado = TOTALMENTE_ABERTO;
        }
        break;
        
      case FECHANDO:
        P1OUT = P1OUT | BIT0; // liga o Led Vermelho
        P1OUT = P1OUT &~ BIT6;  // Desliga o Led Verde
        
        if(botao == 0)
        {
          __delay_cycles(500000);
          estado = PARADO_APOS_FECHANDO;
        }
        
        if(SF == 1 && SA == 0)
        {
          estado = TOTALMENTE_FECHADO;
        }
        break;
        
      case PARADO_APOS_ABRINDO:
        P1OUT = P1OUT &~ BIT0; // Desliga o Led Vermelho
        P1OUT = P1OUT &~ BIT6; // Desliga o Led Verde
        
        if(botao == 0)
        {
          __delay_cycles(500000);
          estado = FECHANDO;
        }
        break;
        
      case PARADO_APOS_FECHANDO:
        P1OUT = P1OUT &~ BIT0; // Desliga o Led Vermelho
        P1OUT = P1OUT &~ BIT6; // Desliga o Led Verde
        
        if(botao == 0)
        {
          __delay_cycles(500000);
          estado = ABRINDO;
        }
        break;
        
      default: 
        break;
    }
  }
}
