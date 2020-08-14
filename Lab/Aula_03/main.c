#include "msp430g2553.h"

#define ON 1  //Definindo ON como valor "1"
#define OFF 0  //Definindo OFF como valor "0"
#define ABRINDO 0  //DE
#define FECHANDO 1
#define ABERTO 2
#define FECHADO 3
#define PARADO_ABRINDO 4
#define PARADO_FECHANDO 5

unsigned char SA = 0; // Totalmente aberto
unsigned char SF = 0; // Totalmente fechado
unsigned char estado = 3; // Estado do portao


int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  P1DIR |= (BIT0+BIT6); // Setando o bit0 e o bit6
  P1REN |= BIT3;
  P1OUT |= BIT3;
  
  while(1)
  {
    switch(estado)
    {
      // Caso o portao esteja abrindo
      case ABRINDO:
        P1OUT |= BIT0;
        P1OUT &=~ BIT6;
        if(SF == OFF && SA == ON){
          estado = ABERTO;
        }
        if((P1IN&BIT3)==0){
          estado = PARADO_ABRINDO;
        }
        break;
       
        // Caso o portao esteja fechando
       case FECHANDO:
          P1OUT &=~ BIT0;
          P1OUT |= BIT6;
          if(SF == ON && SA == OFF){
            estado = FECHADO;
          }
          if((P1IN&BIT3)==0){
            estado = PARADO_FECHANDO;
          }
        break;
        
        // Caso o portao estja totalmente aberto
       case ABERTO:
          P1OUT &=~ BIT0;
          P1OUT &=~ BIT6;
          if((P1IN&BIT3)==0){
             estado = FECHANDO;
           }
        break;
        
        // Caso o portao esteja totalmente fechado
       case FECHADO:
         P1OUT &=~ BIT0;
         P1OUT &=~ BIT6;
         if((P1IN&BIT3)==0){
           estado = ABRINDO;
         }  
        break;
        
        // Caso o portao estava abrindo e foi parado
       case PARADO_ABRINDO:
         P1OUT &=~ BIT0;
         P1OUT &=~ BIT6;
         if((P1IN&BIT3)==0){
           estado = FECHANDO;
         }
        break;
        
        // Caso o portao estava fechando e foi parado
       case PARADO_FECHANDO:
           P1OUT &= ~BIT0;
           P1OUT &= ~BIT6;
           if((P1IN&BIT3)==0){
             estado = ABRINDO;
           }   
        break; 
        
      default: break;
    }
  }
}
