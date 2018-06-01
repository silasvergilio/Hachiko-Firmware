#include "hachiko_reference.h"
#include "hachiko_estrategias.c"

void config()
{

   /*Definindo a direcao de cada uma das portas, A, B, C, D, E.
1 indica input e 0 output e a ordem funciona da seguinte maneira
e.g.

set_tris_a(a7,a6,a5,a4,a3,a2,a1,a0);

*/

   set_tris_a(0b11111111);
   set_tris_b(0b00000000);
   set_tris_c(0b10111111);
   set_tris_d(0b00000000);
   set_tris_e(0b1111);

//Configuracao dos modulos PWM do PIC18F2431
   setup_power_pwm_pins(PWM_BOTH_ON,PWM_BOTH_ON,PWM_BOTH_ON,PWM_BOTH_ON); // Configura os 4 módulos PWM.
  setup_power_pwm(PWM_FREE_RUN, 1, 0, POWER_PWM_PERIOD, 0, 1,30);

   /*
   FREE RUN eh o modo mais aconselhado para mover motores DC, outros modos podem controlar de maneiras especificas, dividindo a frequencia
   o modo UP/DOWN aparentemente eh mais eficiente, eh preciso mais testes para garantir isso.

   1 indica o postscale usado na saída do PWM, o postscale divide a frequencia que recebe por um número, no caso não estamos dividindo por nada

   0 indica o valor inicial da contagem do Timer responsavel pela geracao do pwm

   POWER_PWM_PERIOD eh o periodo do PWM, isso pode controlar a frequencia do PWM de acordo com o periodo, ele eh dado em ciclos do clock

   0  o compare, ele compara esse valor com o valor do timer para verificar se algum evento especial deveria acontecer

   1 o postscale compare,ele afeta o compare utilizado no parâmetro anterior.

   30 o dead time, ele altera a diferença de tempo entre o ON de um PWM e o OFF de seu complementar.
   TODO: fazer consideracao quanto a esse dead time que poderia ser possivelmente reduzido a fim de conseguir melhores reações do robo
*/
   
   setup_adc_ports(ALL_ANALOG); //Define que todas as portas que possuem conversão A/D serão usadas como conversão A/D
   setup_adc(ADC_CLOCK_INTERNAL); //Utiliza o mesmo clock do PIC para o conversor A/D
   enable_motors(); //Controla a habilitação das duas ponte H, para desabilitar uma delas, basta mudar o valor das constantes ENA e ENB

   disable_interrupts(global); //Habilita interrupcao globais
   enable_interrupts(INT_TIMER1); //Habilita interrupcao do Timer1
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8); //Configura Timer1 e uso o pre scaler para dividir por 8;

   disable_interrupts(INT_TIMER0);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2|RTCC_8_BIT); //Configura Timer1 e uso o pre scaler para dividir por 8;

  enable_interrupts(INT_TIMER5); //Habilita interrupcao do Timer1
   setup_timer_5(T5_INTERNAL|T5_DIV_BY_1);//Configura Timer5 e divide o seu clock por 1.

 
   
   enable_interrupts(INT_RDA); //Habilita interrupção da porta serial


}

//Funçãoo que faz a leitura e tratameno de todos os sensores
void sensores()
{
          //Faz a leitura do canal adc indicado e poe a resposta no bit indicado da palavra indicada (Sensores de linha)
        //  respostaSensores = leituraAdc(100,canal_sensor_linha_1, respostaSensores, 0); //Sensor Frente Direito
       //   respostaSensores = leituraAdc(700,canal_sensor_linha_2, respostaSensores, 1); //Sensor Frente Esquerdo

        //  respostaSensores = leituraAdc(920,canal_sensor_linha_3,respostaSensores,6); //Sensor Tras Esquerdo
        //  respostaSensores = leituraAdc(920,canal_sensor_linha_4,respostaSensores,7); //Sensor Tras Direito

         //(Sensores de distancia)

       if(!debuggingLinha)
       {
       respostaSensores = leituraAdc(500,canalSensorDistanciaDireita, respostaSensores,0);
       respostaSensores = leituraAdc(500,canalSensorDistanciaFrenteDireita, respostaSensores, 1);
       respostaSensores = leituraAdc(500,canalSensorDistanciaFrenteCentro, respostaSensores, 2);
       respostaSensores = leituraAdc(500,canalSensorDistanciaFrenteEsquerda, respostaSensores, 3);
       respostaSensores = leituraAdc(500,canalSensorDistanciaEsquerda, respostaSensores, 4);
       }

       trataSensores2(); //responde com o motor em relacao aos sensores usando uma maquina de estados
}

int8 leituraAdc(int16 threshold, int canal, int8 resposta_leitura, int bit)
{

   switch(canal) //switch para preparar o canal desejado para a conversão A/D
   {
      case 0: set_adc_channel(0); break;
      case 1: set_adc_channel(1); break;
      case 2: set_adc_channel(2); break;
      case 3: set_adc_channel(3); break;
      case 4: set_adc_channel(4); break;
      case 5: set_adc_channel(5); break;
      case 6: set_adc_channel(6); break;
      case 7: set_adc_channel(7); break;
      case 8: set_adc_channel(8); break;
      default: break;
   }

   delay_us(10);            //delay de 10 us importante ser um valor pequeno
   leitura = read_adc();    //Realiza a leitura do canal analogico-digital

   if(debuggingLinha)
   {
   if(canal == canal_sensor_linha_1)  printf("*P%Lu*",leitura);
   else if(canal == canal_sensor_linha_2) printf("*G%Lu*",leitura);
   else if(canal == canal_sensor_linha_3) printf("*T%Lu*", leitura);
   else if(canal == canal_sensor_linha_4) printf("*K%Lu*", leitura);
   }

   if (leitura > threshold) bit_clear(resposta_leitura,bit); //Caso o sensor retorne 0, a palavra recebe 0 em seu primeiro bit
   else bit_set(resposta_leitura,bit); //Caso sensor retorne 1 , a palavra recebe 1 em seu primeiro bit

   return resposta_leitura; //devolve a variavel com seus bits alterados

}

int1 leituraAdc_linha(int16 threshold, int canal)
{

   switch(canal) //switch para preparar o canal desejado para a conversão A/D
   {
      case 0: set_adc_channel(0); break;
      case 1: set_adc_channel(1); break;
      case 2: set_adc_channel(2); break;
      case 3: set_adc_channel(3); break;
      case 4: set_adc_channel(4); break;
      case 5: set_adc_channel(5); break;
      case 6: set_adc_channel(6); break;
      case 7: set_adc_channel(7); break;
      case 8: set_adc_channel(8); break;
      default: break;
   }

   delay_us(10);            //delay de 10 us importante ser um valor pequeno
   leitura = read_adc();    //Realiza a leitura do canal analogico-digital

   if (leitura > threshold) return 0; //Caso o sensor retorne 0, a palavra recebe 0 em seu primeiro bit
   else return 1; //Caso sensor retorne 1 , a palavra recebe 1 em seu primeiro bit

}



  void trataSensores2() //Trata a resposta aos sensores usando uma esp�cie de maquina de estados
{

 /*
     Diagramacao dos bits da palavra respostaSensores

     bit 0 - Sensor Linha Direita
     bit 1 - Sensor Linha Esquerda
     bit 2 - Sensor Lateral Direita
     bit 3 - Sensor Frente Direita
     bit 4 - Sensor Frente Esquerda
     bit 5 - Sensor Lateral Esquerda
     bit 6 - Sensor Linha Tras Esquerda
     bit 7 - Sensor Linha Tras Direita
     */

  /*
      if ( bit_test(respostaSensores,0) ) //Verifica isoladamente os sensores de linha
   {

      if(!debuggingLinha)
      {

      //Anda para tr�s
     motor1(VELOCIDADE_LINHA,'b');
      motor2(VELOCIDADE_LINHA,'b');
      delay_ms(TEMPO_LINHA_TRAS_MS);

      estadoSensores = 3;
      mover = 0;

      bit_clear(respostaSensores,0);
      }
   }



   if ( bit_test(respostaSensores,1) ) //Verifica isoladamente os sensores de linha
   {

   if(!debuggingLinha)
   {
      //Anda para tr�s
     motor1(VELOCIDADE_LINHA,'b');
      motor2(VELOCIDADE_LINHA,'b');
      delay_ms(TEMPO_LINHA_TRAS_MS);

      estadoSensores = 1;
      mover = 0;

      bit_clear(respostaSensores,1);
   }
   }
*/

   if ( bit_test(respostaSensores,6) ) //Verifica isoladamente os sensores de linha
   {

      if(!debuggingLinha)
      {
    // motor1(0,'b');
    //  motor2(0,'b');
    //  delay_ms(200);

      //Anda para tr�s
     motor1(VELOCIDADE_LINHA,'f');
      motor2(VELOCIDADE_LINHA,'f');
      delay_ms(TEMPO_LINHA_TRAS_MS);

      //Para o motor rapidamente

      //Gira o rob�
      estadoSensores = 0;
      mover = 0;

      pre_estrategia_executada = 1;  //Atualiza a variavel indicando que a pre estratégia já foi executada
      bit_clear(respostaSensores,6);
      }
   }

   if ( bit_test(respostaSensores,7) ) //Verifica isoladamente os sensores de linha
   {

      if(!debuggingLinha)
      {
    // motor1(0,'b');
    //  motor2(0,'b');
    //  delay_ms(200);

      //Anda para tr�s
      motor1(VELOCIDADE_LINHA,'f');
      motor2(VELOCIDADE_LINHA,'f');
      delay_ms(TEMPO_LINHA_TRAS_MS);

      //Para o motor rapidamen

      estadoSensores = 0;
      mover = 0;
      pre_estrategia_executada = 1; //Atualiza a variavel indicando que a pre estratégia já foi executada
      bit_clear(respostaSensores,7);

      }
   }



   if(!debuggingLinha) //Verifica se não está em modo Debugging.
   {
   if(!input(pinSensorDistanciaTras)) //Verifica se o sensor de trás viu um oponente
   {
      estadoSensores = 6;
      mover = 0;
      pre_estrategia_executada = 1;
   }
   }


   if(estadoSensores == 0) //Estado em que nenhum sensor foi visto, estado inicial do robo
   {
   disable_interrupts(INT_TIMER5);
   disable_interrupts(INT_TIMER0);
   overflowTimer0 = 0; //Zera o contador de overflows do timer 0.
   //Verifica se a pre estrategia ja foi utilizada
   if(!pre_estrategia_executada)
   {
      switch(preBusca)
      {
       case 0: pNull(); break;
       case 3: pBuscaTras(); break;
       case 4: pZigZag(); break;
       case 5: pGiro(giro); break;
      }
   }

   else //Caso que ja tenha executado a pre estrategia
   {
    //Verifica se a primeira busca ja foi executada
      if(!primeiraBusca)
      {
      //Executa estrategia padrao para localizacao do adversario em qualquer ponto da partida depois de ter achado uma vez

        switch(buscaPadrao)
      {
         case 0: buscaPassiva(0); break;
         case 1: buscaTwister(lado,0); break;
         case 2: buscaAgressiva(); break;
         case 3: buscaPulso(0); break;
      }

      }
      else{

        switch(buscaInicial)
      {
         case 0: buscaPassiva(0); break;
         case 1: buscaTwister(lado,0); break;
         case 2: buscaArco(lado,0); break;
         case 3: buscaAgressiva(); break;
         case 4: buscaPulso(0); break;
      }

      }
   }

    /*Estado 0: Nenhum sensor foi visto.
      Estado 1: Sensor Lateral Direito Acionado
      Estado 2: Ataque
      Estado 3: Sensor Lateral Esquerdo Acionado
      Estado 4: Sensor Frente-Direita Acionado
      Estado 5: Sensor Frente-Esquerda Acionado
      Estado 6: Sensor Trás Acionado

    */

      switch(respostaSensores)
      {
      case 0b00000000: estadoSensores = 0; break;
      case 0b00000001: estadoSensores = 1; break;
      case 0b00000011: estadoSensores = 4; break;
      case 0b00000010: estadoSensores = 4; break;
      case 0b00000110: estadoSensores = 2; break;
      case 0b00000100: estadoSensores = 2; break;
      case 0b00001110: estadoSensores = 2; break;
      case 0b00001100: estadoSensores = 2; break;
      case 0b00001000: estadoSensores = 5; break;
      case 0b00011000: estadoSensores = 5; break;
      case 0b00010000: estadoSensores = 3; break;

      default: estadoSensores = 0; break;
      }
   }

   if(estadoSensores == 1)
   {
      motor1(85,'f');
      motor2(85,'b');
      primeiraBusca = 0;
      disable_interrupts(INT_TIMER5);
      disable_interrupts(INT_TIMER1); //Timer para estrategia inicial de busca
      timer(TEMPO_ESTADO_GIRO);

     // printf("Estado Atual: %d \r",estadoSensores);
      if(overflowTimer0 >= overflowsAcao)
      {
      overflowTimer0 = 0;
      estadoSensores = 0;
      }
      else
      {
      switch(respostaSensores)
      {
      case 0b00000110: estadoSensores = 2; break;
      case 0b00000100: estadoSensores = 2; break;
      case 0b00001110: estadoSensores = 2; break;
      case 0b00001100: estadoSensores = 2; break;

      default: estadoSensores = 1;  break;
      }
      }
   }

   if(estadoSensores == 2)
   {
      //Move ambos motores para o araque
      motor1(VELOCIDADE_ATAQUE,'f');
      motor2(VELOCIDADE_ATAQUE,'f');
      //Indica que o robo ja achou um adversario uma vez pelo menos
      primeiraBusca = 0;
      overflowTimer0 = 0;
      //Desabilita interrup��es de giro do motor e de pulso de movimento
      disable_interrupts(INT_TIMER1);
      disable_interrupts(INT_TIMER0);
     
      switch(respostaSensores)
      {
      case 0b00011000: estadoSensores = 2; break;
      case 0b00010000: estadoSensores = 2; break;
      case 0b00001000: estadoSensores = 2; break;
      case 0b00000000: estadoSensores = 0; break;

      case 0b11111110: estadoSensores = 0; respostaSensores = 0b00000000;  break;

      default: estadoSensores = 0; break;

      }

      resposta_sensores_frente = 0b00000000;
   }


   if(estadoSensores == 3)
   {
      motor1(85,'f');
      motor2(85,'b');
      primeiraBusca = 0;
      disable_interrupts(INT_TIMER5);
      disable_interrupts(INT_TIMER1); //Timer para estrategia inicial de busca
      timer(TEMPO_ESTADO_GIRO);

     // printf("Estado Atual: %d \r",estadoSensores);
      if(overflowTimer0 >= overflowsAcao)
      {
        overflowTimer0 = 0;
        estadoSensores = 0;
      }
      else
      {
      switch(respostaSensores)
      {
        case 0b00000110: estadoSensores = 2; break;
        case 0b00000100: estadoSensores = 2; break;
        case 0b00001110: estadoSensores = 2; break;
        case 0b00001100: estadoSensores = 2; break;

        default: estadoSensores = 3;  break;
      }
      }
      
   }

   if(estadoSensores == 4)
   {
      motor1(85,'f');
      motor2(85,'b');
      primeiraBusca = 0;
      disable_interrupts(INT_TIMER1);
      disable_interrupts(INT_TIMER5);
      timer(TEMPO_ESTADO_GIRO_FRENTE); //Configura timer 0 e define limite do overflow

      if(overflowTimer0 >= overflowsAcao)
      {
      overflowTimer0 = 0;
      estadoSensores = 0;
      }

      else
      {
      switch(respostaSensores)
      {
        case 0b00000110: estadoSensores = 2; break;
        case 0b00000100: estadoSensores = 2; break;
        case 0b00001110: estadoSensores = 2; break;
        case 0b00001100: estadoSensores = 2; break;
        default: estadoSensores = 4;  break;
      }
      }
   }

   if(estadoSensores == 5)
   {
      motor1(85,'f');
      motor2(85,'b');
      primeiraBusca = 0;
      disable_interrupts(INT_TIMER1);
      disable_interrupts(INT_TIMER5);
      timer(TEMPO_ESTADO_GIRO_FRENTE); //Configura timer 0 e define limite do overflow

      if(overflowTimer0 >= overflowsAcao)
      {
      overflowTimer0 = 0;
      estadoSensores = 0;
      }

      else
      {
      switch(respostaSensores)
      {
        case 0b00000110: estadoSensores = 2; break;
        case 0b00000100: estadoSensores = 2; break;
        case 0b00001110: estadoSensores = 2; break;
        case 0b00001100: estadoSensores = 2; break;
        default: estadoSensores = 5;  break;
      }
      }
   }
} 

void motor1(int duty_cycle, char sentido)
{
   if(sentido == 'b')
   {
   //gira o motor para frente com duty_cycle% de sua potência total
      set_power_pwm0_duty((int16)((POWER_PWM_PERIOD *4) * (duty_cycle*0.01)));
      set_power_pwm2_duty((int16)(0));

       /*Aqui multiplica-se por 4 pois o maior valor do período do PWM é 4095 e o valor máximo do parâmetro da função,
      que daria 100% de duty cycle é 16383, portanto cria-se assim uma propoção entre o período e a velocidade multiplicando por um número entre 0 e 1
      que seria a porcentagem desejada.
      */
   }

   if(sentido == 'f')
   {
     //gira o motor para trás com duty_cyle% de sua potência total
      set_power_pwm0_duty((int16)(0));
      set_power_pwm2_duty((int16)((POWER_PWM_PERIOD *4) * (duty_cycle*0.01)));
     /*Aqui multiplica-se por 4 pois o maior valor do período do PWM é 4095 e o valor máximo do parâmetro da função,
      que daria 100% de duty cycle é 16383, portanto cria-se assim uma propoção entre o período e a velocidade multiplicando por um número entre 0 e 1
      que seria a porcentagem desejada.
      */
   }
}

void motor2(int duty_cycle, char sentido)
{
   if(sentido == 'b')
   {
    //gira o motor para frente com duty_cycle% de sua potencia total
      set_power_pwm4_duty((int16)((POWER_PWM_PERIOD *4) * (duty_cycle*0.01)));
      set_power_pwm6_duty((int16)(0));
      /*Aqui multiplica-se por 4 pois o maior valor do período do PWM é 4095 e o valor máximo do parâmetro da função,
      que daria 100% de duty cycle é 16383, portanto cria-se assim uma propoção entre o período e a velocidade multiplicando por um número entre 0 e 1
      que seria a porcentagem desejada.
      */
   }

   if(sentido == 'f')
   {
    //gira o motor para tras com duty_cyle% de sua potencia total
      set_power_pwm4_duty((int16)(0));
      set_power_pwm6_duty((int16)((POWER_PWM_PERIOD *4) * (duty_cycle*0.01)));
      /*Aqui multiplica-se por 4 pois o maior valor do período do PWM é 4095 e o valor máximo do parâmetro da função,
      que daria 100% de duty cycle é 16383, portanto cria-se assim uma propoção entre o período e a velocidade multiplicando por um número entre 0 e 1
      que seria a porcentagem desejada.
      */
   }
}


 void bob() //Fun��o para teste completo do rob�
{


   if(estado_teste == 0) //Condi��o para que a mensagem apare�a apenas uma vez
   {
      printf("\nTeste: Funcionamento do Motor Esquerdo - Frente\n"); //Mensagem indicando qual parte do teste o usu�rio est�
      estado_teste = 1; //Atualiza estado
      motor2(0,'f'); // Para motor por seguran�a.
     motor1(0,'f'); //Para motor por seguran�a.
   }

  if(estado_teste == 1)
  {
     motor1(velocidade_teste,'f'); //Testa motor 1 girando em um sentido

  }
   if(estado_teste == 2) //Condi��o para que a mensagem apare�a apenas uma vez
   {
      printf("\nTeste: Funcionamento do Motor Esquerdo - Tras\n"); //Mensagem indicando qual parte do teste o usu�rio est�
      estado_teste = 3; //Atualiza estado
      motor2(0,'f'); // Para motor por seguran�a.
     motor1(0,'f'); //Para motor por seguran�a.
   }

   if(estado_teste == 3)
   {
     motor1(velocidade_teste,'b'); //Testa motor 1 girando em um sentido

   }

    if(estado_teste == 4) //Condi��o para que a mensagem apare�a apenas uma vez
   {
      printf("\nTeste: Funcionamento do Motor Direito - Frente\n"); //Mensagem indicando qual parte do teste o usu�rio est�
      estado_teste = 5; //Atualiza estado
      motor2(0,'f'); // Para motor por seguran�a.
     motor1(0,'f'); //Para motor por seguran�a.
   }

   if(estado_teste == 5)
   {
      motor2(velocidade_teste,'f'); //Testa motor 1 girando em um sentido

   }

    if(estado_teste == 6) //Condi��o para que a mensagem apare�a apenas uma vez
   {
      printf("\nTeste: Funcionamento do Motor Direito - Tras\n"); //Mensagem indicando qual parte do teste o usu�rio est�
      estado_teste = 7; //Atualiza estado
      motor2(0,'f'); // Para motor por seguran�a.
     motor1(0,'f'); //Para motor por seguran�a.
   }

   if(estado_teste == 7)
   {
      motor2(velocidade_teste,'b'); //Testa motor 1 girando em um sentido

   }
   if(estado_teste == 8) //Condi��o para que a mensagem apare�a apenas uma vez
   {
      printf("\nTeste: Motores Desligados\n"); //Mensagem indicando qual parte do teste o usu�rio est�
      estado_teste = 9; //Atualiza estado
      motor2(0,'f'); // Para motor por seguran�a.
     motor1(0,'f'); //Para motor por seguran�a.
   }
      if(estado_teste == 9)
   {
     motor1(0,'b'); //Testa motor 1 girando em um sentido
      motor2(0,'b');

   }
  }

void enable_motors()
{
   if(ENA_state) output_high(PIN_B6); //Manda sinal para habilitar a ponteH do motor 1.
   else output_low(PIN_B6); //Manda sinal para desabilitar a ponteH do motor 1.

   if(ENB_state) output_high(PIN_B7); //Manda sinal para habilitar a ponteH do motor 2.
   else output_low(PIN_B7); //Manda sinal para desabilitar a ponteH do motor 2.

}

void evasao()
{
  disable_interrupts(INT_TIMER5);

   motor1(0,'b');
   motor2(0,'b');
   delay_ms(500);
  //Da um 'passo' para tras do robo adversario
  motor1(VELOCIDADE_EVASAO,'b');
   motor2(VELOCIDADE_EVASAO,'b');
   delay_ms(200);

   //Pare novamente
  motor1(0,'b');
   motor2(0,'b');
   delay_ms(500);

  motor1(35,'b');
   motor2(35,'f');
   delay_ms(150);

}

void imprimeEstrategia(int buscaInicial, int buscaPadrao, int preBusca)
{

         switch(preBusca)
         {
            case 0: printf("\rNull\r"); break;
            case 1: printf("Arco Tras Esquerda\r"); break;
            case 2: printf("Arco Tras Direita\r"); break;
            case 3: printf("Tras Reto\r"); break;
            case 4: printf("Zig Zag\r"); break;
            case 5: printf("Giro\r"); break;
            default: printf("Estrategia Invalida\r"); LIGA = 0; primeiro_inicio = 0; break;
         }

         switch(buscaInicial)
         {
            case 0: printf("Busca Passiva\r"); break;
            case 1: printf("Busca Twister\r"); break;
            case 2: printf("Busca Arco\r"); break;
            case 3: printf("Busca Agressiva\r"); break;
            case 4: printf("Busca Pulso Movimento \r"); break;
            default: printf("Estrategia Invalida\r"); LIGA = 0; primeiro_inicio = 0; break;
         }


         switch(buscaPadrao)
         {
            case 0: printf("Busca Passiva\r"); break;
            case 1: printf("Busca Twister\r"); break;
            case 2: printf("Busca Agressiva\r"); break;
            case 3: printf("Busca Pulso\r"); break;
            default: printf("Estrategia Invalida!\r"); LIGA = 0; primeiro_inicio = 0; break;
         }
  
          printf("Lado: %c\r",lado);

}

void set_estrategia(int codigo_estrategia)
{
   preBusca = (codigo_estrategia/15);
   buscaInicial = (codigo_estrategia/3) - ( (codigo_estrategia/15)*5 );
   buscaPadrao = (codigo_estrategia - ((codigo_estrategia/15)*15)) - (((codigo_estrategia - ((codigo_estrategia/15)*15))/3)*3);
}

void resposta_sensores_frente2()
{

   resposta_sensores_frente = leituraAdc(500,canalSensorDistanciaFrenteDireita, resposta_sensores_frente,0);
   resposta_sensores_frente = leituraAdc(500,canalSensorDistanciaFrenteCentro, resposta_sensores_frente,1);
   //resposta_sensores_frente = leituraAdc(500,canalSensorDistanciaFrenteEsquerda, resposta_sensores_frente,2);

  if(  bit_test(resposta_sensores_frente,0) || bit_test(resposta_sensores_frente,1) ||  bit_test(resposta_sensores_frente,2) )
  {
      bit_set(respostaSensores,3);
      bit_set(respostaSensores,4);
  }

  else
  {
      bit_clear(respostaSensores,3);
      bit_clear(respostaSensores,4);
  }

}


void timer(long tempo_ms)
{
   enable_interrupts(INT_TIMER0);
   overflowsAcao = tempo_ms*10;
}
