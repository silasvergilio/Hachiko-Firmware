/*
Firmware para PIC18F4431 do Robo Hachiko da equipe FEG-Robotica
Data da Ultima atualizacaoo 26/05/2018
Responsavel atual: Silas Vergilio
Versao 1.0
*/
#include<18f4431.h> //Adiciona biblioteca referente ao microcontrolador adequado(PIC15F4431)
#device adc=10 //Define a resolucao do conversor A/D do microcontrolador
#fuses HS, NOWDT, NOPROTECT, NOBROWNOUT, NOPUT, NOLVP //Define algumas Configuraoes do PIC
#use delay(clock=20000000) //definicao do cristal externo usado(na linha superior foi definido usar um cristal rapid HS), em Hz, no caso 20Mhz

/*
HS - Significa que estamos usando um cristal de alta velocidade (no caso 20Mhz)
NOWDTH - Nao desejamos nos usar do WatchDog Timer, que impede que o programa "trave" dentro de loops infinitos ou algo assim. Nosso rob� depende de um loop infinitos
NOPROTECT - O codigo nao e protegido,portanto é possivel acessar ele a partir do microcontrolador
NOLVP - Desabilita programacao em baixa tensao.
*/
//Configuracao da comunicacao rs232 usada para comunicar com o modulo Bluetooth
#use rs232(BAUD=9600,UART1, XMIT=PIN_C6, RCV=PIN_C7, PARITY= N, BITS = 8, STREAM = BT)
/*
Baud Rate é a taxa de transferencia de bits da comunicacao, e importante que quem recebe e quem envia os faca a mesma taxa.
XMIT define o pino que ira transmitir informacoes
RCV define o pino que ira receber informacoes
PARITY eh o tipo de paridade que sera usada na comunicacao
STREAM define a id usada nas funcoes da comunicacao serial
Bits = define o tamanho maximo da cada palavra da comunicacao
*/
#include "hachiko_reference.c" //Inclui bibliotea de funcoes para o metalgarurumon
#include<stdlib.h>
#PRIORITY INT_TIMER0,INT_RDA,INT_TIMER1,INT_TIMER5 //Define as prioridades do das interrupcoes

#INT_RDA
void RDA_isr(void)
{

  if(LIGA == 0)
  {

   //Captacao da palavra que define todas as estrategias do robo
     for(i = 0; i<13;i++)
     {
         stringEstrategia[i] = getc(BT);
     }


     charPreBusca = stringEstrategia[0];
     charBuscaInicial = stringEstrategia[1];
     charBuscaPadrao = stringEstrategia[2];
     charDebuggingLinha = stringEstrategia[9];

     preBusca = charPreBusca - 48;

     buscaInicial = charBuscaInicial - 48;

     buscaPadrao = charBuscaPadrao - 48;

     debuggingLinha = charDebuggingLinha - 48;

     for(i = 0;i<3;i++)
     {
      charVelocidadeAtaque[i] = stringEstrategia[i+3];
     }
     charVelocidadeAtaque[3] = '\0';


      for(i = 0;i<3;i++)
     {
      charVelocidadeTwister[i] = stringEstrategia[i+6];
     }
     charVelocidadeAtaque[3] = '\0';


      for(i = 0;i<3;i++)
     {
      charGiro[i] = stringEstrategia[i+10];
     }

     charGiro[3] = '\0';

     VELOCIDADE_ATAQUE = atoi(charVelocidadeAtaque);
     printf("\r Velocidade De Ataque: %u \r",VELOCIDADE_ATAQUE);

     VELOCIDADE_BUSCA_AGRESSIVA = atoi(charVelocidadeTwister);

     VELOCIDADE_TWISTER = atoi(charVelocidadeTwister);
     printf("\r Velocidade Busca: %u \r",VELOCIDADE_TWISTER);


     giro = atoi(charGiro);
     printf("\r Tempo Giro: %u \r",giro);

     printf("\r Debuggin Linha: %u\r",debuggingLinha);
     //leitura_bt_int = atoi(leitura_bt); //Converte valor da string em um numero inteiro
     LIGA = 1; //oficialmente liga o robo
    // sair = 0;
     primeiro_inicio = 1; //define estado de busca do robo
     //set_estrategia(leitura_bt_int); //separa as 3 estrategias a aprtir do valor enviado na string
     imprimeEstrategia(buscaInicial, buscaPadrao, preBusca); //imprime as estrategias selecionadas
     printf("\rEscolha o lado do oponente:\r"); //pede que o operador selecione o lado para realmente ligar o robo
     lado = getc(); //recebe o char com o lado em que o oponente se encontra
  }
  //Caso o robo esteja em operacao regular.
  if(LIGA == 2)
  {

   leitura_bt = getc(BT);

   if(leitura_bt == 'b')
   {
      LIGA = 0;
    //  sair = 1;
   }

  }
}

//Diretivas de Interrupção Externa para os sensores de linha frontais
#int_EXT1
void EXT1_isr(void)
{
   //Anda para tras
      motor1(VELOCIDADE_LINHA,'b');
      motor2(VELOCIDADE_LINHA,'b');
      delay_ms(TEMPO_LINHA_TRAS_MS);

      estadoSensores = 3;
      mover = 0;

      bit_clear(respostaSensores,0);     
}

#int_EXT2
void EXT2_isr(void)
{
   //Anda para tras
      motor1(VELOCIDADE_LINHA,'b');
      motor2(VELOCIDADE_LINHA,'b');
      delay_ms(TEMPO_LINHA_TRAS_MS);

      estadoSensores = 1;
      mover = 0;

      bit_clear(respostaSensores,0);
}


#int_TIMER1 //deritiva de programacao que indica que o  codigo abaixo eh acionado cada vez que o contador Timer1 da uma a flag de overflow
void TIMER1_isr (void)
{
    overflowTimer1++; //Variavel que conta quantas vezes o timer1 deu overflow

    set_timer1(0); //Reseta o Timer para que ele faca uma nova contagem

    if(overflowTimer1 == 20) //O contador da overflow aproximadamente 20 vezes em 2 segundos (valor hipotetico)
    {
  //  printf("Interrupt \n");
    overflowTimer1 = 0;
    mover = 1;
       }
}
#int_TIMER5 //Timer para tempo máximo de execução de uma estratégia
void TIMER5_isr(void)
{
   //TODO Timer para controlar o tempo de execução de uma estratégia
  overflowTimer5++;

}
//Timer responsavel por impedir que o robo fique muito tempo no mesmo estado
#int_TIMER0 //Diretiva de programacoa que indica que o codigo abaixo eh acionado cada vez que o contador Timer0 da uma flag de overflow
void TIMER0_isr (void)
{
   overflowTimer0++;
  // printf("timer0: %Lu \r",overflowTimer0);
   set_timer0(0); //Reseta o Timer para que ele faca uma nova contagem
}
void main()
{

   config(); //Executa rotina de configuracoes

   disable_interrupts(INT_TIMER0);
   disable_interrupts(INT_TIMER1);
   disable_interrupts(INT_TIMER5);

   while(true) //Ciclo principal do software
   {
    while(LIGA == 0)
    {
    //Desliga os motores quando o robo e desligado via celular
    motor1(0,'f');
    motor2(0,'f');
    if(primeiro_inicio == 0)
    {
      printf("\rDigite a string responsavel pela estrategia\r");
      primeiro_inicio = 1;
    }
    //Liga dois leds para indicar que o robo esta ligado
    output_high(LED_0);
    output_low(LED_1);
    //Desabilita interrupcoes quando o robo nao foi acionado
    disable_interrupts(INT_TIMER1);
    disable_interrupts(INT_TIMER0);
    disable_interrupts(INT_TIMER5);
    disable_interrupts(INT_EXT1);
    disable_interrupts(INT_EXT2);
    enable_interrupts(INT_RDA);
    enable_interrupts(global);

    //Reseta as variaveis de controle do overflow dos Timers
    overflowTimer1 = 0;
    overflowTimer0 = 0;
    overflowTimer5 = 0;
    primeiraBusca = 1;
    pre_estrategia_executada = 0;
    }

   while(LIGA == 1)
   {
      if(primeiro_inicio == 1)
         {
         if(primeiro_inicio == 1) primeiro_inicio = 2;
         }
         LIGA = 2;
   }

   while(LIGA == 2)
   {
         disable_interrupts(INT_TIMER0);
         disable_interrupts(INT_TIMER1);
         disable_interrupts(INT_TIMER5);
        
         if(primeiro_inicio == 2)
         {

         set_timer0(0); //Inicializa o timer0
         set_timer1(0); //Inicializa o timer1
         set_timer5(0); //Inicializa o timer5
         primeiro_inicio = 0;
      //   printf("\r Lado: %c \r",lado);
       // delay_ms(3000);
         }
         //Liga o Led que indica que o rob� esta ligado
         clear_interrupt(int_EXT2);
         clear_interrupt(int_EXT1);
          ext_int_edge(1,H_TO_L); //Configura a interrupção externa como borda de descida.
          ext_int_edge(2,H_TO_L); //Configura a interrupção externa como borda de descida.
         enable_interrupts(INT_EXT1);
         enable_interrupts(INT_EXT2);

         output_high(LED_1);
         output_low(LED_0);

         //Função Principal do Programa
         sensores();

        

} //While LIGA


   }
}
