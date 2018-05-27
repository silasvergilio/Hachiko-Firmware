#include "hachiko_reference.h"

void buscaPassiva(int tempoEstrategia)
{
        tempoEstrategia*=77; //Transforma o tempo lido em segundos para o valor equivalente de overflows do timer 5
                             //Cada Overflow do timer5 é equivalente a 13ms aproximadamente 1/13ms =~77.
         //Para os dois motores
         motor1(0,'b');
         motor2(0,'f');
         //Forca o motor a nao dar um pulso de movimento
         mover = 0;
         //desabilita o timer responsavel por impedir que o robo fique num mesmo estado muito tempo
         disable_interrupts(INT_TIMER0);
         //reseta o contador do timer0
         overflowTimer0 = 0;

         //Define tempo limite 
         if(tempoEstrategia == 0)
         {
            disable_interrupts(INT_TIMER5);
         }

         else
         {
            enable_interrupts(INT_TIMER5);
            if(overflowTimer5 >= tempoEstrategia)
            {
               primeiraBusca = 0;
               overflowTimer5 = 0;
            }

         }
         
}

void buscaTwister(char lado, int tempoEstrategia)
{
      tempoEstrategia*=77; //Transforma o tempo lido em segundos para o valor equivalente de overflows do timer 5
                             //Cada Overflow do timer5 é equivalente a 13ms aproximadamente 1/13ms =~77.
      //Gira de acordo com o lado mais proximo do robo adversario
        if(lado == 'e')
        {
        motor1(VELOCIDADE_TWISTER,'b');
        motor2(VELOCIDADE_TWISTER,'f');
        }
        else if(lado == 'd')
        {
        motor1(VELOCIDADE_TWISTER,'f');
        motor2(VELOCIDADE_TWISTER,'b');
        }

         //Forca o motor a nao dar um pulso de movimento
         mover = 0;
         //desabilita o timer responsavel por impedir que o robo fique num mesmo estado muito tempo
         disable_interrupts(INT_TIMER0);
         //reseta o contador do timer0
         overflowTimer0 = 0;

         //Define tempo limite 
         if(tempoEstrategia == 0)
         {
            disable_interrupts(INT_TIMER5);
         }

         else
         {
            enable_interrupts(INT_TIMER5);
            if(overflowTimer5 >= tempoEstrategia)
            {
               primeiraBusca = 0;
               overflowTimer5 = 0;
            }

         }
}

void buscaArco(char lado,int tempoEstrategia)
{

      tempoEstrategia*=77; //Transforma o tempo lido em segundos para o valor equivalente de overflows do timer 5
                             //Cada Overflow do timer5 é equivalente a 13ms aproximadamente 1/13ms =~77.
      //Gira de acordo com o lado mais proximo do adversario
      if(lado == 'e')
      {
      motor1(30,'f');
      motor2(60,'f');
      }
      else if (lado == 'd')
      {
      motor1(60,'f');
      motor2(30,'f');
      }

         //Forca o motor a nao dar um pulso de movimento
         mover = 0;
         //desabilita o timer responsavel por impedir que o robo fique num mesmo estado muito tempo
         disable_interrupts(INT_TIMER0);
         //reseta o contador do timer0
         overflowTimer0 = 0;

         //Define tempo limite 
         if(tempoEstrategia == 0)
         {
            disable_interrupts(INT_TIMER5);
         }

         else
         {
            enable_interrupts(INT_TIMER5);
            if(overflowTimer5 >= tempoEstrategia)
            {
               primeiraBusca = 0;
               overflowTimer5 = 0;
            }

         }
}

void buscaAgressiva()
{
      //Para os dois motores
         motor1(VELOCIDADE_BUSCA_AGRESSIVA,'f');
         motor2(VELOCIDADE_BUSCA_AGRESSIVA,'f');
         //Forca o motor a nao dar um pulso de movimento
         mover = 0;
         //desabilita o timer responsavel por impedir que o robo fique num mesmo estado muito tempo
         disable_interrupts(INT_TIMER0);
         disable_interrupts(INT_TIMER5);
         //reseta o contador do timer0
         overflowTimer0 = 0;
}

void buscaPulso(int tempoEstrategia)
{
      tempoEstrategia*=77; //Transforma o tempo lido em segundos para o valor equivalente de overflows do timer 5
                             //Cada Overflow do timer5 é equivalente a 13ms aproximadamente 1/13ms =~77.
      enable_interrupts(INT_TIMER1); //Habilita Timer1 para dar o pulso de movimento
      disable_interrupts(INT_TIMER5);
      //Para os motores quando nao tiver enviado o pulso
      motor1(0,'f');
      motor2(0,'f');
      if(mover) //Executa um pequeno movimento caso tenha dado overflows o suficiente
      {
         motor1(50,'f');
         motor2(50,'f');
         delay_ms(50);
      //  printf("Moveu\n");
         mover = 0;
      }
      //Define tempo limite 
         if(tempoEstrategia == 0)
         {
            disable_interrupts(INT_TIMER5);
         }

         else
         {
            enable_interrupts(INT_TIMER5);
            if(overflowTimer5 >= tempoEstrategia)
            {
               primeiraBusca = 0;
               overflowTimer5 = 0;
            }

         }
}

void pBuscaTras()
{
 //manda ambos motores para tras em recuo
   motor1(50,'b');
   motor2(50,'b');
 //executa recuo por 50ms
   //TODO: verificar condicao de continuar recuo com sensor traseiro.
}

void pNull()
{
   motor1(0,'b');
   motor2(0,'b');
   pre_estrategia_executada = 1;
}

void pZigZag()
{
   //Vira para um lado
   motor1(30,'f');
   motor2(30,'b');
   delay_ms(400);

   //Para para observacao
   motor1(0,'f');
   motor2(0,'f');
   delay_ms(1000);

   //Vai para frente
   motor1(30,'f');
   motor2(30,'f');
   delay_ms(400);

      //Para para observacao
   motor1(0,'f');
   motor2(0,'f');
   delay_ms(1000);


   //Vira para outro lado
   motor1(30,'b');
   motor2(30,'f');
   delay_ms(400);

   //Para para observacao
   motor1(0,'f');
   motor2(0,'f');
   delay_ms(1000);

   //Vai para frente
   motor1(30,'f');
   motor2(30,'f');
   delay_ms(400);

   //Para para observacao
   motor1(0,'f');
   motor2(0,'f');
   delay_ms(1000);

   pre_estrategia_executada = 1;
}

void pGiro(int ang)
{

   if(lado == 'd')
   {

   //Gira robo
   motor1(100,'f');
   motor2(100,'b');
   delay_ms(ang*2);//controla o angulo
   motor1('f',0);
   motor2('f',0);
   }

   if(lado == 'e')
   {
   //Gira robo
   motor1(100,'b');
   motor2(100,'f');
   delay_ms(ang*2); //Controla o angulo
   motor1('f',0);
   motor2('f',0);
   }

   pre_estrategia_executada = 1;
}
