#include "msp430g2553.h"
#include "lcd.h"

unsigned int cont = 0;

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  P1REN |= BIT3;
  P1OUT |= BIT3;
  
  InitLCD();
              
  for(;;)
  {
    if(((P1IN & BIT3) == 0) && (cont < 10 )){
      LCDPrintXYStr(5,1, "CONTANDO");
      cont++;
      LCDPrintXYVal(8,2,cont);
      __delay_cycles(250000);
    }
    if(cont >= 10){
      __delay_cycles(2000000);
      LCDPrintXYStr(1,1,"LIMITE ATINGIDO");
      LCDPrintXYStr(1,2,"REINICIE SISTEMA");
    }
  }
  
}
