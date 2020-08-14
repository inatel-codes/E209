#include "msp430g2553.h"
#include <stdio.h>

char mensagem_tx[20];
char mensagem_rx[32];
int tam=0;
int TAMANHO = 1;
int cont=0;
int valorAD;

void configUART(void);
void UART_TX(char * tx_data);
void configTimer0(void);
void configTimer1(void);
void configADC();
unsigned int leADC(unsigned int porta, unsigned int canal);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  P1DIR |= BIT0;
  P1DIR |= BIT6;
  P1SEL |= BIT6;
  P1DIR &=~BIT3;
  P1OUT |= BIT3;
  P1REN |= BIT3;
  
  configUART();
  configADC();
  configTimer1();
  
  _BIS_SR(GIE);

  
  P1OUT &=~ BIT0;
  P1OUT &= ~BIT6;
  
  while(1) // While 1 is equal to 1 (forever)
  {
    if((mensagem_rx[0] == 'A'))
    {
        configTimer0();
        unsigned char x = 0;
        if(!((P1IN & BIT3)==BIT3)) // Was button pressed?
        {
            UART_TX("valorAD"); // If yes, Transmit message & drink beer
            __delay_cycles(100000); //Debounce button so signal is not sent multiple times
            x++;
            //sprintf: mensagem que salva o texto convertido, sendo
            // 1o parametro = destino onde sera salva a string
            // 2o parâmetro = mensagem de texto com as variáveis, onde
            // %c=variavel char, %d=variável decimal %u=variável inteira sem sinal
            // %f=variável float(nao suportada pela biblioteca padrão do IAR)
            // %.xd=variável inteira em decimal, com X dígitos, ou seja, %.4d= decimal com 4 digitos
            sprintf(mensagem_tx,"<%.3d>\n\r",x);
            UART_TX(mensagem_tx);
         }
    }
    else if((mensagem_rx[0] == 'M'))
    {
      
    }
 
  }
}

void configUART(void){
  P1SEL = BIT1+BIT2; // Seleciona os pinos P1.1 e P1.2 para serem UART
  P1SEL2 = BIT1+BIT2; // Seleciona os pinos P1.1 e P1.2 para serem UART
  UCA0CTL1 |= UCSSEL_2; // Utilizar clck principal
  UCA0BR0 = 104; // baudrate 9600
  UCA0BR1 = 0; // baudrate 9600
  UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST; // Inicia UART
  IE2 |= UCA0RXIE;
}

//Função de envio de mensagens “STRINGS”
void UART_TX (char * tx_data) 
{
  unsigned int i=0;
  while(tx_data[i]) // Espera enviar todos caracteres da STRING
  {
    while ((UCA0STAT & UCBUSY)); //espera terminar o envio da ultima informação
    UCA0TXBUF = tx_data[i]; // envia o elemento na posição i
    i++; // incrementa posição do vetor
  }
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

// ROTINA DE INTERRUPÇÃO DO TIMER0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
  cont++;
  if(cont > 30){
      valorAD = leADC(BIT4,INCH_4);
      if(valorAD < 512){
          //TA1CCR1 = 49999;
          P1OUT &= ~ BIT0;
          P1OUT |= BIT6;
          cont = 0;
      }
      else if(valorAD > 513){
          P1OUT |= BIT0;
          P1OUT &= ~ BIT6;
          //TA1CCR1 = 0;
          cont = 0;
      }
  }
    
  TA0CCTL0 &=~ CCIFG; // LIMPANDO A FLAG DE INTERRUPÇÃO
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  mensagem_rx[tam] = UCA0RXBUF; // TX -> RXed character
  
  //utilize este trecho para mensagens com tamanho maior que 1 caracter por exemplo a palavra
  //"Olá" que usa 3 caracteres
  tam++;
  if(tam == TAMANHO){
  tam=0;
  }
  
  IFG2=IFG2&~UCA0RXIFG;
}
