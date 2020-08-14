#include "msp430g2553.h"
#include <stdio.h>

char mensagem_tx[20];
char mensagem_rx[32];
int tam=0;
int TAMANHO = 3;
int valor=0;

void configUART(void);

void UART_TX(char * tx_data);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  P1DIR &=~BIT3;
  P1OUT |= BIT3;
  P1REN |= BIT3;
  
  configUART();
  _BIS_SR(GIE);
  
  int x = 0;
  
  while(x==0){
      if((mensagem_rx[0] == 'o') && (mensagem_rx[1] == 'l') && (mensagem_rx[2] == 'a')){
        x=1;
      }
  }
  
  x=0;
  
  do{
  valor = (mensagem_rx[0]-48)*100 + (mensagem_rx[1]-48)*10 +(mensagem_rx[2]-48)*1;
      if(valor == 250){
        x=1;
      }
  }while(x==0);
  
  x=0;
  
  while(1) // While 1 is equal to 1 (forever)
  {
    if(!((P1IN & BIT3)==BIT3)) // Was button pressed?
    {
      UART_TX("Hello World! \r\n"); // If yes, Transmit message & drink beer
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

void UART_TX (char * tx_data) //Função de envio de mensagens “STRINGS”
{
  unsigned int i=0;
  while(tx_data[i]) // Espera enviar todos caracteres da STRING
  {
    while ((UCA0STAT & UCBUSY)); //espera terminar o envio da ultima informação
    UCA0TXBUF = tx_data[i]; // envia o elemento na posição i
    i++; // incrementa posição do vetor
  }
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