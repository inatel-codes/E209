#include "msp430g2553.h"

unsigned char LED1 = 0;
unsigned char LED2 = 0;

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_8MHZ;
  DCOCTL = CALDCO_8MHZ;
  
  for(;;)
  {
    LED1 = 1;
    LED2 = 0;
    __delay_cycles(5000000);
    LED1 = 0;
    LED2 = 1;
    __delay_cycles(5000000);
  }
  

}
