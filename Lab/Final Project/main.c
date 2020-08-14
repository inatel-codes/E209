#include "msp430g2553.h"

// FUNCAO MAIN
void main( void )
{
  // DESABILITANDO O WATCHDOG 
  WDTCTL = WDTPW + WDTHOLD;
  
  configTimer0();
  configTimer1();
  configInterrupt();
  configADC();
  
  _BIS_SR(GIE); // HABILITANDO INTERRUP플O GLOBAL
  LPM1;
  
  for(;;){
    
  }
}

// FUNCAO DE CONFIGURACAO DO TIMER0
void configTimer0(void){
  TA0CTL |= MC_1;
  TA0CTL |= TASSEL_2;
  TA0CTL |= ID_1;
  TA0CCR0 |= 49999;
  TA0CCTL0 |= CCIE;
}

// FUNCAO DE CONFIGURACAO DO TIMER1 COMO PWM
void configTimer1(void){
  TA1CCTL1 = OUTMOD_7;
  TA1CCR0 = 49999;
  TA1CCR1 = 0;
  TA1CTL = TASSEL_2; + ID_0 + MC_1;
  TA1CCTL0 &= ~CCIE;
}

// FUNCAO DE CONFIGURACAO DE INTERRUPCAO
void configInterrupt(){  
  P1IES |= BIT3;
  P1IE |= BIT3;
  P1IFG &=~ BIT3;
}

// FUNCAO DE CONFIGURACAO DO CONVERSOR AD
void configADC()
{
  ADC10CTL1 |= SHS_0 + ADC10SSEL_3;
  ADC10CTL0 = SREF_0 + ADC10ON;
}

// FUNCAO PARA LER O CONVERSOR AD
unsigned int leADC(unsigned int porta, unsigned int canal)
{
  unsigned long int valor = 0;
  int media;
  ADC10AE0 |= porta;
  
  ADC10CTL0 &=~  ADC10ON;
  ADC10CTL1 &=~ INCH_7;
  ADC10CTL1 |= canal;
  ADC10CTL0 |=  ADC10ON;
  
  for(int i = 0;i<100;i++){
    ADC10CTL0 |= ENC + ADC10SC;
    while((ADC10CTL0 & ADC10IFG) == 0);
    valor += ADC10MEM;
  }
  
   ADC10CTL0 &= ~ADC10IFG;
   valor = valor/100;
   media = (int) valor;
   return media;
}

// FUNCAO PARA CONFIGURAR O UART
void configUART(void)
{
  P1SEL = BIT1 + BIT2; 
  P1SEL2 = BIT1 + BIT2; 
  UCA0CTL1 |= UCSSEL_2; 
  UCA0BR0 = 104; 
  UCA0BR1 = 0;
  UCA0MCTL = UCBRS0; 
  UCA0CTL1 &= ~UCSWRST; 
  IE2 |= UCA0RXIE; 
}

// FUNCAO PARA ENVIAR MENSAGENS
void UART_TX (char * tx_data) 
{
  unsigned int i=0;
  while(tx_data[i]) 
  {
    while ((UCA0STAT & UCBUSY)); 
    UCA0TXBUF = tx_data[i];
    i++; 
  }
} 

// ROTINA DE INTERRUP플O DO TIMER0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
  TA0CCTL0 &=~ CCIFG; // LIMPANDO A FLAG DE INTERRUP플O
}

// ROTINA DE INTERRUP플O DO TIMER1
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
  TA1CCTL0 &=~ CCIFG; // LIMPANDO A FLAG DE INTERRUP플O
}

// ROTINA DE INTERRUP플O EXTERNA
#pragma vector = PORT1_VECTOR
__interrupt void INTERRUPCAO_EXTERNA_ISR(void)
{
  P1IFG &=~ BIT3; // LIMPANDO A FLAG DE INTERRUP플O
}

// ROTINA DE INTERRUP플O PARA RECEP플O DE MENSAGENS
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  mensagem_rx[tam] = UCA0RXBUF; 
  
  tam++;
  if(tam == TAMANHO)
  {
    tam=0;
  }

  IFG2=IFG2&~UCA0RXIFG;
} 
